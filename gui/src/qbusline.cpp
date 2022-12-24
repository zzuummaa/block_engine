#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>

#include "qbusline.h"

QBusLine::QBusLine(qreal length, QWidget* parent) : QWidget(parent), length(length) {}

void QBusLine::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, static_cast<int>(length), 1);
    QWidget::paintEvent(event);
    painter.end();
}
