#pragma once

#include <QWidget>

class QBusLine : public QWidget {
Q_OBJECT
public:
    explicit QBusLine(qreal length, QWidget* parent);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    qreal length;
};
