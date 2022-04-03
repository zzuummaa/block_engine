#include "blockenginewindow.h"
#include "ui_blockenginewindow.h"

BlockEngineWindow::BlockEngineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlockEngineWindow)
{
    ui->setupUi(this);
}

BlockEngineWindow::~BlockEngineWindow()
{
    delete ui;
}
