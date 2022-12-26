#pragma once

#include <QWidget>

class QBusLine : public QWidget {
Q_OBJECT
public:
    constexpr static const qreal MIN_BUS_LINE_LENGTH = 2.0;

    explicit QBusLine(qreal length, QWidget* parent = nullptr);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    qreal length;
};
