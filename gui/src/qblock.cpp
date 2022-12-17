#include <qpainter.h>
#include <qevent.h>

#include "qblock.h"

QBlock::QBlock(
    BlockTypeInfo info,
    BusGroupHolder inputs)
    : block_info(std::move(info))
    , inputs(std::move(inputs)) {

    setFixedSize(50, 50);
//    inputs.forEach([&](QBus* bus){
//        bus->setParent(this);
//    });

    // TODO add busses https://stackoverflow.com/questions/15413564/make-qgraphicsproxywidget-movable-selectable


}

void QBlock::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::red);
    QWidget::paintEvent(event);
    painter.end();
}