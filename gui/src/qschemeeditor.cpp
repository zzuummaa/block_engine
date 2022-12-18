#include "qschemeeditor.h"
#include <qgraphicsitem.h>
#include <qgraphicsproxywidget.h>
#include <qdebug.h>
#include <QTimeLine>
#include <qscrollbar.h>

QSchemeEditor::QSchemeEditor(QWidget* parent) : QGraphicsView(parent), numScheduledScalings(0), scale(1.0) {
    auto scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::gray));
//    scene->setSceneRect(-50000, -50000, 100000, 100000);
//    scene->addEllipse(-500, -500, 1000, 1000, QPen(Qt::black), QBrush(Qt::NoBrush));

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setScene(scene);
}

void QSchemeEditor::addBlock(QBlock* block) {
    // Create the graphics item that will be used to move the widget around the screen as well as be selectable (for example in case we want to delete a widget that is in the scene)
    // Depending on the position of the graphics item relative to its widget proxy you can adjust the size and location of both
    QGraphicsRectItem *proxyControl = scene()->addRect(0, 0, block->width(), block->height(), QPen(Qt::transparent), QBrush(Qt::NoBrush)); // widget->width() works properly here because of the resize(layout->sizeHint()) that we have used inside it
    proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);

    // Create the proxy by adding the widget to the scene
    QGraphicsProxyWidget * const proxy = scene()->addWidget(block);
    // In my case the rectangular graphics item is supposed to be above my widget so the position of the widget is shifted along the Y axis based on the height of the rectangle of that graphics item
//    proxy->setPos(0, 0+proxyControl->rect().height());
    proxy->setParentItem(proxyControl);

    QObject::connect(block, &QBlock::busStartMarked, this, &QSchemeEditor::startBus);
    QObject::connect(block, &QBlock::busEndMarked, this, &QSchemeEditor::endBus);

    model.addBlock(reinterpret_cast<SchemeEditorModel::TId>(block), block->info());
    block->forEachInput([&](auto pin){
        model.addInput(
            reinterpret_cast<SchemeEditorModel::TId>(block),
            reinterpret_cast<SchemeEditorModel::TId>(pin),
            pin->info());
    });
    block->forEachOutput([&](auto pin){
        model.addOutput(
            reinterpret_cast<SchemeEditorModel::TId>(block),
            reinterpret_cast<SchemeEditorModel::TId>(pin),
            pin->info());
    });
}

void QSchemeEditor::updateSceneRect() {
    const auto editorBounds = mapToScene(rect()).boundingRect();
    const auto itemBounds = scene()->itemsBoundingRect();

//    scene()->setSceneRect(
//        itemBounds.x() - editorBounds.width(),
//        itemBounds.y() - editorBounds.height(),
//        itemBounds.width() + editorBounds.width() * 2,
//        itemBounds.height() + editorBounds.height() * 2);

//    scene()->setSceneRect(
//        editorBounds.x() - editorBounds.width(),
//        editorBounds.y() - editorBounds.height(),
//        editorBounds.width() * 3,
//        editorBounds.height() * 3);

    const qreal padding = 4;

    scene()->setSceneRect(
        center.x() - editorBounds.width() / 2.0 + padding,
        center.y() - editorBounds.height() / 2.0 + padding,
        editorBounds.width() - padding * 2,
        editorBounds.height() - padding * 2);

//    qDebug() << "QSchemeEditor::updateSceneRect() bounds: " << editorBounds << ", center:" << editorBounds.center();
}

void QSchemeEditor::resizeEvent(QResizeEvent* event) {
//    center = rect().center();
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

    qDebug() << "QSchemeEditor::wheelEvent() numDegrees:" << numDegrees << ", numScheduledScalings:" << numScheduledScalings;

    auto anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

void QSchemeEditor::scalingTime(qreal x) {
    qreal factor = 1.0 + qreal(numScheduledScalings) / 300.0;
    scale *= factor;
//    qreal factor = 2.0;
    qDebug() << "factor:" << factor;
//    scale(factor, factor);

    auto transform = QTransform(scale, 0, 0, scale, center.x(), center.y());

//    auto transform = this->transform();
//    const auto oldCenter = transform.map(center);
//    transform.scale(factor, factor);
//    const auto newCenter = transform.map(rect().center());
//    transform.translate(oldCenter.x() - newCenter.x(), oldCenter.y() - newCenter.y());

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
    if (event->button() == Qt::RightButton) {
        origin_x = event->x();
        origin_y = event->y();
    }

    QGraphicsView::mousePressEvent(event);
}

void QSchemeEditor::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::RightButton) {
//        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - origin_x));
//        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - origin_y));

        auto start = QPoint(event->x(), event->y());
        auto end = QPoint(origin_x, origin_y);
        auto diff = mapToScene(end) - mapToScene(start);
        center += diff;
        qDebug() << "center:" << center << ", diff:" << diff;

        origin_x = event->x();
        origin_y = event->y();
    }

    if (event->buttons() & (Qt::LeftButton|Qt::RightButton|Qt::MiddleButton)) {
        updateSceneRect();
    }

    QGraphicsView::mouseMoveEvent(event);
}

// =========================== Bus creating ===========================

void QSchemeEditor::startBus(QPin* pin) {
    qDebug() << "QSchemeEditor::startBus()" << pin;
}

void QSchemeEditor::endBus(QPin* pin) {
    qDebug() << "QSchemeEditor::endBus()" << pin;
}
