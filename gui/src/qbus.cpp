//
// Created by Stepan on 11.12.2022.
//

#include <qpainter.h>
#include <qevent.h>

#include "qbus.h"

QBus::QBus(const BusTypeInfo& info) : QWidget(), info(info) {
    setFixedSize(4, 4);
}

void QBus::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.setBrush(Qt::white);
    painter.begin(this);
    painter.drawEllipse(event->rect());
    QWidget::paintEvent(event);
    painter.end();
}
