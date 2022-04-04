#include "qblockenginewindow.h"
#include "ui_qblockenginewindow.h"
#include "gui/block_factory.h"

using namespace block_engine::gui;

QBlockEngineWindow::QBlockEngineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlockEngineWindow)
{
    ui->setupUi(this);
    for (const auto& [type_info, initializer] : make_block_factory().map) {
        ui->block_list->addItem(type_info.name);
    }
}

QBlockEngineWindow::~QBlockEngineWindow()
{
    delete ui;
}
