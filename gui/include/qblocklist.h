//
// Created by Stepan on 10.12.2022.
//

#ifndef BLOCK_ENGINE_QBLOCKLIST_H
#define BLOCK_ENGINE_QBLOCKLIST_H

#include <qlistwidget.h>
#include "block_factory.h"

class QBlockList : public QListWidget {
Q_OBJECT
public:
    explicit QBlockList(QWidget* parent = nullptr, const BlockFactory& factory = make_block_factory());
    void setBlockFactory(const BlockFactory& factory);

signals:
    void blockDoubleClicked(const BlockFactory::TBlockInitializer& initializer);

private:
    BlockFactory block_factory;
};

#endif //BLOCK_ENGINE_QBLOCKLIST_H
