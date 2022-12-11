#include <qpainter.h>
#include <qevent.h>

#include "qblock.h"

QBlock::QBlock(const BlockTypeInfo& info) : block_info(info) {
    setFixedSize(50, 50);

}

void QBlock::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::red);
    QWidget::paintEvent(event);
    painter.end();
}