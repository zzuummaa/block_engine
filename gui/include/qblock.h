#ifndef BLOCK_ENGINE_QBLOCK_H
#define BLOCK_ENGINE_QBLOCK_H

#include <QWidget>

class QBlock : public QWidget {
Q_OBJECT
public:
    explicit QBlock(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif //BLOCK_ENGINE_QBLOCK_H
