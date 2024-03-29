#include <qdebug.h>

#include "qblockenginewindow.h"
#include "ui_qblockenginewindow.h"
#include "block_factory.h"

using namespace block_engine;
using namespace block_engine::core;

QBlockEngineWindow::QBlockEngineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BlockEngineWindow)
{
    ui->setupUi(this);

    auto calcIsEnd = std::make_shared<std::atomic<bool>>();

    core = std::make_unique<Core>(std::make_shared<CoutPrintEventHandler>(calcIsEnd));

    QObject::connect(ui->menubar, &QMenuBar::triggered, this, [this, calcIsEnd](QAction* action){
        qDebug() << "QMenuBar::triggered()" << action->text();
        if (action->text() == "Старт") {
            if (*calcIsEnd) {
                core->onSetScheme(ui->scheme_editor->model.getScheme());
                core->onStartCalc();
            }
        }
    });

    QObject::connect(ui->block_list, &QBlockList::blockDoubleClicked, ui->scheme_editor, [this](const auto& initializer){
        ui->scheme_editor->sceneController->addBlock(initializer.makeBlock(), QPointF());
    });
}

QBlockEngineWindow::~QBlockEngineWindow()
{
    core->onStopCalc();
    delete ui;
}
