#ifndef BLOCK_ENGINE_QBLOCK_H
#define BLOCK_ENGINE_QBLOCK_H

#include <QWidget>
#include "block_type_info.h"


class QBlock : public QWidget {
Q_OBJECT
public:
    explicit QBlock(const BlockTypeInfo& info);

    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    BlockTypeInfo block_info;
};

#endif //BLOCK_ENGINE_QBLOCK_H
