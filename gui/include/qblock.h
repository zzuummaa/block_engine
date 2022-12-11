#ifndef BLOCK_ENGINE_QBLOCK_H
#define BLOCK_ENGINE_QBLOCK_H

#include <QWidget>
#include <bus_group_holder.h>
#include <block_type_info.h>


class QBlock : public QWidget {
Q_OBJECT
public:
    explicit QBlock(BlockTypeInfo info, BusGroupHolder inputs);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    BlockTypeInfo block_info;
    BusGroupHolder inputs;
};

#endif //BLOCK_ENGINE_QBLOCK_H
