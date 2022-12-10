#include "qschemeeditor.h"
#include <qgraphicsitem.h>
#include <qgraphicsproxywidget.h>
#include <qdebug.h>
#include <QTimeLine>
#include <qscrollbar.h>

QSchemeEditor::QSchemeEditor(QWidget* parent) : QGraphicsView(parent) {
    auto scene = new QGraphicsScene(this);
    scene->setSceneRect(-1000, -1000, 1000, 1000);

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setScene(scene);
}

void QSchemeEditor::addBlock(QBlock* block) {
    qDebug() << "QSchemeEditor::addBlock";

//    QGraphicsProxyWidget* proxyWidget = scene()->addWidget(block);
//    proxyWidget->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);

    // Create the graphics item that will be used to move the widget around the screen as well as be selectable (for example in case we want to delete a widget that is in the scene)
    // Depending on the position of the graphics item relative to its widget proxy you can adjust the size and location of both
    QGraphicsRectItem *proxyControl = scene()->addRect(0, 0, block->width(), block->height(), QPen(Qt::black), QBrush(Qt::NoBrush)); // widget->width() works properly here because of the resize(layout->sizeHint()) that we have used inside it
    proxyControl->setFlag(QGraphicsItem::ItemIsMovable, true);
    proxyControl->setFlag(QGraphicsItem::ItemIsSelectable, true);
    // Create the proxy by adding the widget to the scene
    QGraphicsProxyWidget * const proxy = scene()->addWidget(block);
    // In my case the rectangular graphics item is supposed to be above my widget so the position of the widget is shifted along the Y axis based on the height of the rectangle of that graphics item
//    proxy->setPos(0, 0+proxyControl->rect().height());
    proxy->setParentItem(proxyControl);
}

// ================ Scaling begin ===========================

void QSchemeEditor::wheelEvent(QWheelEvent* event) {
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // see QWheelEvent documentation

    num_scheduled_scalings += numSteps;
    if (num_scheduled_scalings * numSteps < 0) { // if user moved the wheel in another direction, we reset previously scheduled scalings
        num_scheduled_scalings = numSteps;
    }

    QTimeLine *anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

void QSchemeEditor::scalingTime(qreal x) {
    qreal factor = 1.0+ qreal(num_scheduled_scalings) / 300.0;
    scale(factor, factor);
}

void QSchemeEditor::animFinished() {
    if (num_scheduled_scalings > 0) {
        num_scheduled_scalings--;
    }
    else
    num_scheduled_scalings++;
    sender()->~QObject();
}

// ================ Scaling end ===========================
