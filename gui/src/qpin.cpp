#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>

#include <utility>

#include "qpin.h"

QPin::QPin(BusTypeInfo info) : pin_info(std::move(info)) {
    setFixedSize(SIZE);
    setMouseTracking(true);

    // TODO uncomment for transparent background, then fix "QWidget::paintEngine: Should no longer be called" message
//    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_TranslucentBackground);
//    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_TransparentForMouseEvents);
}

void QPin::paintEvent(QPaintEvent* event) {
    QPainter painter;
    painter.begin(this);
    painter.setBrush(Qt::transparent);
    painter.drawEllipse(event->rect());
    QWidget::paintEvent(event);
    painter.end();
}

void QPin::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        emit pinPressed();
        return;
    }

    QWidget::mousePressEvent(event);
}

void QPin::mouseMoveEvent(QMouseEvent* event) {
//    qDebug() << "QPin::mouseMoveEvent()" << this;
    emit pinFocussed();

    QWidget::mouseMoveEvent(event);
}

