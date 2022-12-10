#ifndef BLOCK_ENGINE_QSCHEMEEDITOR_H
#define BLOCK_ENGINE_QSCHEMEEDITOR_H

#include <QGraphicsView>
#include "qblock.h"

class QSchemeEditor : public QGraphicsView  {
    Q_OBJECT
public:
    explicit QSchemeEditor(QWidget* parent = nullptr);

    void addBlock(QBlock* block);

protected:
    void wheelEvent ( QWheelEvent * event ) Q_DECL_OVERRIDE;

protected slots:
    void scalingTime(qreal x);
    void animFinished();

private:
    int num_scheduled_scalings;
    int origin_x;
    int origin_y;
};

#endif //BLOCK_ENGINE_QSCHEMEEDITOR_H
