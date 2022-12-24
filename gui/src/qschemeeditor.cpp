#include <qgraphicsitem.h>
#include <qgraphicsproxywidget.h>
#include <qdebug.h>
#include <QTimeLine>
#include <qscrollbar.h>
#include <variant>
#include <cmath>

#include "qschemeeditor.h"

using TItemVariant = std::variant<QGraphicsItem*, QPointF, QPin*>;

template<class... Ts>
struct Visitor : Ts... {
    using Ts::operator()...;
};

template<class... Ts>
Visitor(Ts...) -> Visitor<Ts...>;

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

static inline QDebug operator<<(QDebug stream, const TItemVariant& original) {
    std::visit(Visitor{
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

QSchemeEditor::QSchemeEditor(QWidget* parent) : QGraphicsView(parent), isShiftPressed(false), numScheduledScalings(0), scale(1.0) {
    auto scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::gray));
//    scene->setSceneRect(-50000, -50000, 100000, 100000);
//    scene->addEllipse(-500, -500, 1000, 1000, QPen(Qt::black), QBrush(Qt::NoBrush));

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setScene(scene);

    QObject::connect(&pinLinker, &QPinLinkDetector::link, this, [this](QPin* from, QPin* to){
//        qDebug() << "link from" << from << "to" << to;
        if (model.addLink(reinterpret_cast<SchemeEditorModel::TId>(from), reinterpret_cast<SchemeEditorModel::TId>(to))) {
            // old way
        }
    });
}

void QSchemeEditor::addBlock(QBlock* block) {
    auto pair = addSceneProxy(block, 0, 0);
    auto proxy = pair.second;

    QObject::connect(block, &QBlock::pinPressed, this, &QSchemeEditor::pinPressed);
    QObject::connect(block, &QBlock::pinFocussed, this, [this](auto pin){ pinFocussed(pin, proxyByPin[pin]->geometry()); });

    model.addBlock(reinterpret_cast<SchemeEditorModel::TId>(block), block->info());
    block->forEachInput([&](auto pin){
        proxyByPin.emplace(pin, proxy->createProxyForChildWidget(pin));

        model.addInput(
            reinterpret_cast<SchemeEditorModel::TId>(block),
            reinterpret_cast<SchemeEditorModel::TId>(pin),
            pin->info());
    });
    block->forEachOutput([&](auto pin){
        proxyByPin.emplace(pin, proxy->createProxyForChildWidget(pin));

        model.addOutput(
            reinterpret_cast<SchemeEditorModel::TId>(block),
            reinterpret_cast<SchemeEditorModel::TId>(pin),
            pin->info());
    });
}

void QSchemeEditor::tryAddBusLine(QPointF from, QPointF to) {
    auto diff = to - from;
    qreal len = std::sqrt(std::pow(diff.x(), 2) + std::pow(diff.y(), 2));

    auto busLine = new QBusLine(len);
    auto bus = new QBus(busLine);
    busses.emplace(busLine, bus);

    auto [proxyControl, proxy] = addSceneProxy(busLine, from.x(), from.y());

    double angle = std::atan2(diff.y(), diff.x()) / M_PI * 180.0;
    proxyControl->setTransform(QTransform().rotate(angle));
}

std::pair<QGraphicsRectItem*, QGraphicsProxyWidget*> QSchemeEditor::addSceneProxy(QWidget* widget, qreal x, qreal y) {
    // Create the graphics item that will be used to move the widget around the screen as well as be selectable (for example in case we want to delete a widget that is in the scene)
    // Depending on the position of the graphics item relative to its widget proxy you can adjust the size and location of both
    QGraphicsRectItem* proxyControl = scene()->addRect(0, 0, widget->width(), widget->height(), QPen(Qt::transparent), QBrush(Qt::NoBrush)); // widget->width() works properly here because of the resize(layout->sizeHint()) that we have used inside it
    proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);
    proxyControl->setPos(x, y);

    // Create the proxy by adding the widget to the scene
    QGraphicsProxyWidget* const proxy = scene()->addWidget(widget);
    // In my case the rectangular graphics item is supposed to be above my widget so the position of the widget is shifted along the Y axis based on the height of the rectangle of that graphics item
//    proxy->setPos(0, 0+proxyControl->rect().height());
    proxy->setParentItem(proxyControl);

    return std::make_pair(proxyControl, proxy);
}

void QSchemeEditor::updateSceneRect() {
    const auto editorBounds = mapToScene(rect()).boundingRect();
    const auto padding = mapToScene(0, 0, 4, 4).boundingRect().size();

    scene()->setSceneRect(
        center.x() - editorBounds.width() / 2.0 + padding.width(),
        center.y() - editorBounds.height() / 2.0 + padding.height(),
        editorBounds.width() - padding.width() * 2,
        editorBounds.height() - padding.height() * 2);

//    qDebug() << "QSchemeEditor::updateSceneRect() bounds: " << editorBounds << ", center:" << editorBounds.center();
}

void QSchemeEditor::resizeEvent(QResizeEvent* event) {
    updateSceneRect();
    QGraphicsView::resizeEvent(event);
}

// =========================== Scene scaling ===========================

void QSchemeEditor::wheelEvent(QWheelEvent* event) {
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // see QWheelEvent documentation

    numScheduledScalings += numSteps;
    if (numScheduledScalings * numSteps < 0) { // if user moved the wheel in another direction, we reset previously scheduled scalings
        numScheduledScalings = numSteps;
    }

    auto anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

void QSchemeEditor::scalingTime(qreal) {
    qreal factor = 1.0 + qreal(numScheduledScalings) / 300.0;
    scale *= factor;

    auto transform = QTransform(scale, 0, 0, scale, center.x(), center.y());

    setTransform(transform);

    updateSceneRect();
}

void QSchemeEditor::animFinished() {
    if (numScheduledScalings > 0) {
        numScheduledScalings--;
    } else {
        numScheduledScalings++;
    }
    sender()->~QObject();
}

// =========================== Scene moving ===========================

void QSchemeEditor::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "QSchemeEditor::mousePressEvent()";
        if (isShiftPressed) {
            return;
        }
        leftPressPos = event->pos();
    } else if (event->button() == Qt::RightButton) {
        origin_x = event->x();
        origin_y = event->y();
    }

    QGraphicsView::mousePressEvent(event);
}

void QSchemeEditor::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::RightButton) {
        auto start = QPoint(event->x(), event->y());
        auto end = QPoint(origin_x, origin_y);
        auto diff = mapToScene(end) - mapToScene(start);
        center += diff;
//        qDebug() << "center:" << center << ", diff:" << diff;

        origin_x = event->x();
        origin_y = event->y();
    }

    if (event->buttons() & (Qt::LeftButton|Qt::RightButton|Qt::MiddleButton)) {
        updateSceneRect();
    }

    QGraphicsView::mouseMoveEvent(event);
}

// =========================== Bus creating ===========================

void QSchemeEditor::pinPressed(QPin* pin) {
//    qDebug() << "QSchemeEditor::pinPressed()" << pin;
    pinLinker.pinPressed(pin);
}

void QSchemeEditor::pinFocussed(QPin* pin, QRectF pinRect) {
//    qDebug() << "QSchemeEditor::pinFocussed()" << pinRect;
    pinLinker.mouseMoved(pin, mapFromScene(pinRect).boundingRect());
}

void QSchemeEditor::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        qDebug() << "QSchemeEditor::mouseReleaseEvent()";

        if (!leftPressPos) {
            return;
        }

        // empty - empty
        // pin - empty
        // pin - pin
        // bus - empty
        // bus - pin
        // bus - bus

        auto fromItem = getOriginal(itemAt(*leftPressPos), mapToScene(*leftPressPos));
        auto toItem = getOriginal(itemAt(event->pos()), mapToScene(event->pos()));

        qDebug() << "movement from" << fromItem << "to" << toItem;

        std::visit(Visitor{
            [&](QGraphicsItem*, QGraphicsItem*) {},
            [&](QGraphicsItem*, QPointF) {},
            [&](QGraphicsItem*, QPin*) {},

            [&](QPin*, QGraphicsItem*) {},
            [&](QPin*, QPointF) {},
            [&](QPin*, QPin*) {},

            [&](QPointF, QGraphicsItem*) {},
            [&](QPointF point1, QPointF point2) { tryAddBusLine(point1, point2); },
            [&](QPointF, QPin*) {},
        }, fromItem, toItem);

        leftPressPos = std::nullopt;
//        if (!item) {
//            qDebug() << "release without item";
//        } else if (auto proxy = dynamic_cast<QGraphicsProxyWidget*>(item)) {
//            auto widget = proxy->widget();
//            if (auto pin = dynamic_cast<QPin*>(widget)) {
//                qDebug() << "release pin" << pin;
//            } else {
//                qDebug() << "release unexpected widget" << widget->metaObject()->className();
//            }
//        } else if (auto object = item->toGraphicsObject()) {
//            qDebug() << "release unexpected item" << object->metaObject()->className();
//        } else {
//            qDebug() << "release unexpected item";
//        }

        pinLinker.mouseReleased(event->pos());
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void QSchemeEditor::keyPressEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Shift) {
        isShiftPressed = true;
        leftPressPos = std::nullopt;
    }
    QGraphicsView::keyPressEvent(event);
}

void QSchemeEditor::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Shift) {
        isShiftPressed = false;
    }
    QGraphicsView::keyReleaseEvent(event);
}

QPinLinkDetector::QPinLinkDetector() : state(State::Ready), pressedPin(nullptr) {}

void QPinLinkDetector::pinPressed(QPin* pin) {
    if (state != State::Ready && state != State::MouseReleased) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    pressedPin = pin;
    state = State::PinPressed;
//    qDebug() << "QPinLinker PinPressed";
}

void QPinLinkDetector::mouseReleased(QPoint pos) {
    if (state != State::PinPressed) {
        reset();
        return;
    }
    releasePosition = pos;
    state = State::MouseReleased;
//    qDebug() << "QPinLinker MouseReleased at" << releasePosition;
}

void QPinLinkDetector::mouseMoved(QPin* pin, QRectF pinRect) {
    if (state != State::MouseReleased) {
        return;
    }
    if (!pressedPin) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
//    qDebug() << "QPinLinker mouseMoved to" << pinRect;
    if (pinRect.contains(releasePosition)) {
        emit link(pressedPin, pin);
    }
    reset();
}

void QPinLinkDetector::reset() {
    state = State::Ready;
    pressedPin = nullptr;
    releasePosition = QPoint();
}
