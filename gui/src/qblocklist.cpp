#include <QTimer>

#include "qblocklist.h"

QBlockList::QBlockList(QWidget* parent, const BlockFactory& factory) : QListWidget(parent) {
    setBlockFactory(factory);

    QObject::connect(this, &QBlockList::itemDoubleClicked, this, [this](QListWidgetItem *item){
        emit blockDoubleClicked({item->text()}, block_factory.getInitializerByBlockName(item->text()));
    });

    QTimer::singleShot(10, this, [this](){
        emit blockDoubleClicked({"sum_int"}, block_factory.getInitializerByBlockName("sum_int"));
    });
}

void QBlockList::setBlockFactory(const BlockFactory& factory) {
    block_factory = factory;
    clear();
    for (const auto& [type_info, initializer] : block_factory.map) {
        addItem(type_info.name);
    }
}
