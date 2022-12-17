//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <qwidget.h>
#include <bus_type_info.h>

class QPin : public QWidget {
Q_OBJECT
public:
    static constexpr auto SIZE = QSize(4, 4);

    explicit QPin(BusTypeInfo);

    const BusTypeInfo& info() { return pin_info; }

protected:
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void busStartMarked();
    void busEndMarked();

private:
    BusTypeInfo pin_info;
};