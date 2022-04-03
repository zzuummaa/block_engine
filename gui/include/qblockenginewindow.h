#ifndef BLOCK_ENGINE_QBLOCKENGINEWINDOW_H
#define BLOCK_ENGINE_QBLOCKENGINEWINDOW_H

#include <QMainWindow>

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
};

#endif //BLOCK_ENGINE_QBLOCKENGINEWINDOW_H
