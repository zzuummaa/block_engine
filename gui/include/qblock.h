#ifndef BLOCK_ENGINE_QBLOCK_H
#define BLOCK_ENGINE_QBLOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <bus_group_holder.h>
#include <block_type_info.h>

namespace Ui {
class QBlock;
}

class QBlock : public QWidget {
Q_OBJECT
public:
    explicit QBlock(BlockTypeInfo info, BusGroupHolder inputs);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::QBlock *ui;

    BlockTypeInfo block_info;
    BusGroupHolder inputs;
};

#endif //BLOCK_ENGINE_QBLOCK_H
