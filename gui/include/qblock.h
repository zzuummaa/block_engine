#ifndef BLOCK_ENGINE_QBLOCK_H
#define BLOCK_ENGINE_QBLOCK_H

#include <QWidget>
#include <QVBoxLayout>
#include <pin_group_holder.h>
#include <block_type_info.h>

namespace Ui {
class QBlock;
}

class QBlock : public QWidget {
Q_OBJECT
public:
    explicit QBlock(BlockTypeInfo info, PinGroupHolder inputs, PinGroupHolder outputs);

    const BlockTypeInfo& info() { return block_info; }

    template<typename TConsumer>
    void forEachInput(const TConsumer& consumer) {
        inputs.forEach(consumer);
    }

    template<typename TConsumer>
    void forEachOutput(const TConsumer& consumer) {
        outputs.forEach(consumer);
    }

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

signals:
    void pinPressed(QPin* pin);
    void pinFocussed(QPin* pin);

private:
    Ui::QBlock *ui;

    BlockTypeInfo block_info;
    PinGroupHolder inputs;
    PinGroupHolder outputs;

    void addInput(QPin*);
    void addOutput(QPin*);
};

#endif //BLOCK_ENGINE_QBLOCK_H
