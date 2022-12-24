#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>

#include "qbusline.h"

QBusLine::QBusLine(qreal length, QWidget* parent) : QWidget(parent), length(length) {
    setFixedSize(static_cast<int>(length), 1);
}

void QBusLine::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::black);
    QWidget::paintEvent(event);
    painter.end();
}
