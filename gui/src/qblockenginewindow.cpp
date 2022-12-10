#include "qblockenginewindow.h"
#include "ui_qblockenginewindow.h"
#include "block_factory.h"

QBlockEngineWindow::QBlockEngineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlockEngineWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->block_list, &QBlockList::blockDoubleClicked, ui->scheme_editor, [this](const auto& info, const auto& initializer){
        ui->scheme_editor->addBlock(initializer());
    });
}

QBlockEngineWindow::~QBlockEngineWindow()
{
    delete ui;
}
