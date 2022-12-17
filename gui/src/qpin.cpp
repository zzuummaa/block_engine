//
// Created by Stepan on 11.12.2022.
//

#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>

#include <utility>

#include "qpin.h"

QPin::QPin(BusTypeInfo info) : pin_info(std::move(info)) {
    setFixedSize(SIZE);
}

void QPin::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.setBrush(Qt::white);
    painter.drawEllipse(event->rect());
    QWidget::paintEvent(event);
    painter.end();
}

void QPin::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit busStartMarked();
        return;
    }

    QWidget::mousePressEvent(event);
}

void QPin::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit busEndMarked();
        return;
    }

    QWidget::mouseReleaseEvent(event);
}
