#include "qblockenginewindow.h"
#include "ui_qblockenginewindow.h"

QBlockEngineWindow::QBlockEngineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlockEngineWindow)
{
    ui->setupUi(this);
}

QBlockEngineWindow::~QBlockEngineWindow()
{
    delete ui;
}
