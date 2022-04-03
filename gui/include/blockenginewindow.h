#ifndef BLOCK_ENGINE_BLOCKENGINEWINDOW_H
#define BLOCK_ENGINE_BLOCKENGINEWINDOW_H

#include <QMainWindow>

namespace Ui {
class BlockEngineWindow;
}

class BlockEngineWindow : public QMainWindow {
Q_OBJECT

public:
    explicit BlockEngineWindow(QWidget *parent = nullptr);

    virtual ~BlockEngineWindow();

private:
    Ui::BlockEngineWindow *ui;
};

#endif //BLOCK_ENGINE_BLOCKENGINEWINDOW_H
