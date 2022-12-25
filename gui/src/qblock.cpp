#include <qlabel.h>
#include <qpainter.h>
#include <qevent.h>
#include <qdebug.h>

#include "qblock.h"
#include "ui_qblock.h"

QBlock::QBlock(
        BlockTypeInfo info,
        PinGroupHolder inputs_,
        PinGroupHolder outputs_)
    : ui(new Ui::QBlock)
    , block_info(std::move(info))
    , inputs(std::move(inputs_))
    , outputs(std::move(outputs_)) {
    ui->setupUi(this);

    int inputsCount = 0;
    forEachInput([&](QPin* pin){
        inputsCount++;
        addInput(pin);
    });

    int outputsCount = 0;
    forEachOutput([&](QPin* pin){
        outputsCount++;
        addOutput(pin);
    });

    int maxCount = std::max(inputsCount, outputsCount);

    const auto nameSize = ui->nameLabel->fontMetrics().boundingRect(block_info.name);
    auto height = std::max(nameSize.height(), maxCount * (QPin::SIZE.height() + 5) + 5);

    setFixedSize(nameSize.width() + 30, height);
    setContentsMargins(0, 0, 0, 0);

    ui->nameLabel->setText(block_info.name);
}

void QBlock::paintEvent(QPaintEvent *event) {
    QPainter painter;
    painter.begin(this);
    painter.setPen(QPen(Qt::black, 1));
    painter.setBrush(Qt::red);
    painter.drawRect(event->rect());
    QWidget::paintEvent(event);
    painter.end();
}



void QBlock::addInput(QPin* pin) {
    ui->inputsLayout->addWidget(pin);
    QObject::connect(pin, &QPin::pinPressed, this, [this, pin](){ emit pinPressed(pin); });
    QObject::connect(pin, &QPin::pinFocussed, this, [this, pin](){ emit pinFocussed(pin); });
}

void QBlock::addOutput(QPin* pin) {
    ui->outputsLayout->addWidget(pin);
    QObject::connect(pin, &QPin::pinPressed, this, [this, pin](){ emit pinPressed(pin); });
    QObject::connect(pin, &QPin::pinFocussed, this, [this, pin](){ emit pinFocussed(pin); });
}
