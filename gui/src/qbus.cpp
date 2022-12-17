//
// Created by Stepan on 11.12.2022.
//

#include <qpainter.h>
#include <qevent.h>

#include "qbus.h"

QBus::QBus(const BusTypeInfo& info) : info(info) {
    setFixedSize(SIZE);
}

void QBus::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.setBrush(Qt::white);
    painter.drawEllipse(event->rect());
    QWidget::paintEvent(event);
    painter.end();
}
