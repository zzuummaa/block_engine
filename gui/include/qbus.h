//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <qwidget.h>
#include <bus_type_info.h>

class QBus : public QWidget {
Q_OBJECT
public:
    explicit QBus(const BusTypeInfo&);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    BusTypeInfo info;
};