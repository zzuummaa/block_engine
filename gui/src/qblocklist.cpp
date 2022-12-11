//
// Created by Stepan on 10.12.2022.
//

#include "qblocklist.h"

QBlockList::QBlockList(QWidget* parent, const BlockFactory& factory) : QListWidget(parent) {
    setBlockFactory(factory);

    QObject::connect(this, &QBlockList::itemDoubleClicked, this, [this](QListWidgetItem *item){
        emit blockDoubleClicked({item->text()}, block_factory.getInitializerByBlockName(item->text()));
    });
}

void QBlockList::setBlockFactory(const BlockFactory& factory) {
    block_factory = factory;
    clear();
    for (const auto& [type_info, initializer] : block_factory.map) {
        addItem(type_info.name);
    }
}
