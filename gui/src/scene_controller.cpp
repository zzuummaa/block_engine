#include "scheme_editor/scene_controller.h"

#include <cmath>

#include <qdebug.h>
#include <qgraphicsproxywidget.h>
#include <qgraphicsscene.h>

using TItemVariant = std::variant<QGraphicsItem*, QPointF, QPin*>;

template<class... Ts>
struct ItemVisitor : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
ItemVisitor(Ts...) -> ItemVisitor<Ts...>;

static TItemVariant getOriginal(QGraphicsItem* item, QPointF pos) {
    if (!item) {
        return pos;
    }

    if (auto proxy = dynamic_cast<QGraphicsProxyWidget*>(item)) {
        if (auto pin = dynamic_cast<QPin*>(proxy->widget())) {
            return pin;
        }
    }

    return item;
}

static QDebug operator<<(QDebug stream, const TItemVariant& original) {
    std::visit(ItemVisitor{
        [&](QGraphicsItem* item)  {
            stream << "unexpected";
            if (auto proxy = dynamic_cast<QGraphicsProxyWidget*>(item)) {
                stream << "widget" << proxy->widget()->metaObject()->className();
            } else if (auto object = item->toGraphicsObject()) {
                stream << "object" << object->metaObject()->className();
            } else {
                stream << "item";
            }
        },
        [&](QPointF pos)      { stream << pos; },
        [&](QPin* pin)       { stream << pin; }
    }, original);
    return stream;
}

SceneController::SceneController(
    SchemeEditorModel* model,
    QGraphicsScene* scene,
    QObject* parent)
    : QObject(parent)
    , model(model)
    , scene(scene)
    , isShiftPressed(false) {

}

void SceneController::addBlock(QBlock* block, QPointF pos) {
    auto pair = addSceneProxy(block, pos);
    auto proxy = pair.second;

    model->addBlock(reinterpret_cast<SchemeEditorModel::TId>(block), block->info());
    block->forEachInput([&](auto pin){
        if (!proxyByPin.emplace(pin, proxy->createProxyForChildWidget(pin)).second) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        model->addInput(
                reinterpret_cast<SchemeEditorModel::TId>(block),
                reinterpret_cast<SchemeEditorModel::TId>(pin),
                pin->info());
    });
    block->forEachOutput([&](auto pin){
        if (!proxyByPin.emplace(pin, proxy->createProxyForChildWidget(pin)).second) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        model->addOutput(
                reinterpret_cast<SchemeEditorModel::TId>(block),
                reinterpret_cast<SchemeEditorModel::TId>(pin),
                pin->info());
    });
}

static qreal length(QPointF vec) {
    return std::sqrt(std::pow(vec.x(), 2) + std::pow(vec.y(), 2));
}

static QPointF pinCenter(QGraphicsProxyWidget* pin) {
    return pin->mapToScene(pin->rect().center());
}

void SceneController::tryAddBusLine(QPin* pin1, QPin* pin2) {
    auto proxy1It = proxyByPin.find(pin1);
    auto proxy2It = proxyByPin.find(pin2);
    if (proxy1It == proxyByPin.end() || proxy2It == proxyByPin.end()) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    auto proxy1Center = pinCenter(proxy1It->second);
    auto proxy2Center = pinCenter(proxy2It->second);
    if (length(proxy2Center - proxy1Center) < QBusLine::MIN_BUS_LINE_LENGTH) {
        return;
    }

    auto busIt1 = busses.find(pin1);
    auto busIt2 = busses.find(pin2);

    std::shared_ptr<QBus> busPtr;
    if (busIt1 == busses.end() && busIt2 == busses.end()) {
        QBus bus;
        if (!bus.linkPin(pin1) || !bus.linkPin(pin2)) {
            return;
        }

        busPtr = std::make_shared<QBus>(std::move(bus));
        if (!busses.emplace(pin1, busPtr).second
            ||  !busses.emplace(pin2, busPtr).second) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }
    } else if (busIt1 != busses.end() && busIt2 == busses.end()) {
        if (!busIt1->second->linkPin(pin2)) {
            return;
        }

        if (!busses.emplace(pin2, busIt1->second).second) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        busPtr = busIt1->second;

    } else if (busIt1 == busses.end() && busIt2 != busses.end()) {
        if (!busIt2->second->linkPin(pin1)) {
            return;
        }

        if (!busses.emplace(pin1, busIt2->second).second) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        busPtr = busIt2->second;
    } else {
        busPtr = concatBusses(busIt1->second, busIt2->second);
    }

    if (busPtr) {
        if (!model->addLink(reinterpret_cast<SchemeEditorModel::TId>(pin1), reinterpret_cast<SchemeEditorModel::TId>(pin2))) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }
        addBusLine(busPtr, proxy1Center, proxy2Center);
    }
}

void SceneController::tryAddBusLine(QPin* pin, QPointF point) {
    auto proxy = proxyByPin.find(pin);
    if (proxy == proxyByPin.end()) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    auto proxyCenter = pinCenter(proxy->second);
    if (length(point - proxyCenter) < QBusLine::MIN_BUS_LINE_LENGTH) {
        return;
    }

    auto busIt1 = busses.find(pin);

    if (busIt1 == busses.end()) {
        tryAddBusLine(proxyCenter, point);
    } else {
        addBusLine(busIt1->second, proxyCenter, point);
    }
}

void SceneController::tryAddBusLine(QPointF from, QPointF to) {
    if (length(to - from) < QBusLine::MIN_BUS_LINE_LENGTH) {
        return;
    }

    auto bus = std::make_shared<QBus>();
    addBusLine(bus, from, to);
}

void SceneController::addBusLine(const std::shared_ptr<QBus>& bus, QPointF from, QPointF to) {
    auto diff = to - from;
    qreal len = length(diff);

    qDebug() << "SceneController::addBusLine() with len" << len;

    auto busLine = new QBusLine(len);
    if (!bus->linkPart(busLine)) {
        // should be linkable
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    if (!busses.emplace(busLine, bus).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    auto [proxyControl, proxy] = addSceneProxy(busLine, from);

    double angle = std::atan2(diff.y(), diff.x()) / M_PI * 180.0;
    proxyControl->setTransform(QTransform().rotate(angle));
}

std::shared_ptr<QBus> SceneController::concatBusses(const std::shared_ptr<QBus>& bus1, const std::shared_ptr<QBus>& bus2) {
    auto combinedBus = std::shared_ptr<QBus>(QBus::concat(bus1.get(), bus2.get()));
    if (!combinedBus) {
        return combinedBus;
    }

    const auto eraser = [&](auto widget){ busses.erase(widget); };
    bus1->forEachItem(eraser);
    bus2->forEachItem(eraser);

    combinedBus->forEachItem([&](auto widget){
        if (!busses.emplace(widget, combinedBus).second) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }
    });

    return combinedBus;
}

std::pair<QGraphicsRectItem*, QGraphicsProxyWidget*> SceneController::addSceneProxy(QWidget* widget, QPointF pos) {
    // Create the graphics item that will be used to move the widget around the screen as well as be selectable (for example in case we want to delete a widget that is in the scene)
    // Depending on the position of the graphics item relative to its widget proxy you can adjust the size and location of both
    QGraphicsRectItem* proxyControl = scene->addRect(0, 0, widget->width(), widget->height(), QPen(Qt::transparent), QBrush(Qt::NoBrush)); // widget->width() works properly here because of the resize(layout->sizeHint()) that we have used inside it
    proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);
    proxyControl->setPos(pos);

    // Create the proxy by adding the widget to the scene
    QGraphicsProxyWidget* const proxy = scene->addWidget(widget);
    // In my case the rectangular graphics item is supposed to be above my widget so the position of the widget is shifted along the Y axis based on the height of the rectangle of that graphics item
//    proxy->setPos(0, 0+proxyControl->rect().height());
    proxy->setParentItem(proxyControl);

    return std::make_pair(proxyControl, proxy);
}

void SceneController::mousePressEvent(Qt::MouseButton button, QPointF pos, QGraphicsItem* item) {
    qDebug() << "SceneController::mousePressEvent()" << button << pos;
    if (button == Qt::LeftButton) {
        if (isShiftPressed) {
            return;
        }
        leftPressPos = pos;
        leftPressItem = item;
    }

}

void SceneController::mouseReleaseEvent(Qt::MouseButton button, QPointF pos, QGraphicsItem* item) {
    qDebug() << "SceneController::mouseReleaseEvent()" << button << pos;
    if (button == Qt::LeftButton) {
        if (!leftPressPos) {
            return;
        }

        // empty - empty
        // pin - empty
        // pin - pin
        // bus - empty
        // bus - pin
        // bus - bus

        auto fromItem = getOriginal(leftPressItem, *leftPressPos);
        auto toItem = getOriginal(item, pos);

        qDebug() << "movement from" << fromItem << "to" << toItem;

        std::visit(ItemVisitor{
            [&](QGraphicsItem*, QGraphicsItem*) {},
            [&](QGraphicsItem*, QPointF) {},
            [&](QGraphicsItem*, QPin*) {},

            [&](QPin*, QGraphicsItem*) {},
            [&](QPin* pin, QPointF point) { tryAddBusLine(pin, point); },
            [&](QPin* pin1, QPin* pin2) { tryAddBusLine(pin1, pin2); },

            [&](QPointF, QGraphicsItem*) {},
            [&](QPointF point1, QPointF point2) { tryAddBusLine(point1, point2); },
            [&](QPointF point, QPin* pin) { tryAddBusLine(pin, point); },
        }, fromItem, toItem);

        leftPressPos = std::nullopt;
        leftPressItem = nullptr;
    }
}

void SceneController::keyPressEvent(int key) {
    if (key == Qt::Key_Shift) {
        isShiftPressed = true;
        leftPressPos = std::nullopt;
    }
}

void SceneController::keyReleaseEvent(int key) {
    if (key == Qt::Key_Shift) {
        isShiftPressed = false;
    }
}
