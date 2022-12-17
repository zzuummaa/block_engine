#include <qlabel.h>
#include <qpainter.h>
#include <qevent.h>

#include <iostream>

#include "qblock.h"
#include "ui_qblock.h"

QBlock::QBlock(
    BlockTypeInfo info,
    BusGroupHolder inputs_)
    : ui(new Ui::QBlock)
    , block_info(std::move(info))
    , inputs(std::move(inputs_)) {
    ui->setupUi(this);

    int inputsCount = 0;
    inputs.forEach([&](QBus* bus){
        inputsCount++;
        ui->inputsLayout->addWidget(bus);
    });

    const auto nameSize = ui->nameLabel->fontMetrics().boundingRect(block_info.name);
    auto height = std::max(nameSize.height(), inputsCount * (QBus::SIZE.height() + 5) + 5);

    setFixedSize(nameSize.width() + 30, height);
    setContentsMargins(0, 0, 0, 0);

    ui->nameLabel->setText(block_info.name);
}

void QBlock::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.fillRect(event->rect(), Qt::red);
    QWidget::paintEvent(event);
    painter.end();
}