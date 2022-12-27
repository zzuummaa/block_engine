#ifndef BLOCK_ENGINE_QBLOCKENGINEWINDOW_H
#define BLOCK_ENGINE_QBLOCKENGINEWINDOW_H

#include <QMainWindow>
#include <core/core.h>

namespace Ui {
class BlockEngineWindow;
}

class QBlockEngineWindow : public QMainWindow {
Q_OBJECT

public:
    explicit QBlockEngineWindow(QWidget *parent = nullptr);

    virtual ~QBlockEngineWindow();

private:
    Ui::BlockEngineWindow *ui;
    std::unique_ptr<block_engine::core::Core> core;
};

#endif //BLOCK_ENGINE_QBLOCKENGINEWINDOW_H
