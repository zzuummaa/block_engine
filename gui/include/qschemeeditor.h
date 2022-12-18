#ifndef BLOCK_ENGINE_QSCHEMEEDITOR_H
#define BLOCK_ENGINE_QSCHEMEEDITOR_H

#include <QGraphicsView>
#include "qblock.h"
#include "scheme_editor_model.h"

class QSchemeEditor : public QGraphicsView  {
    Q_OBJECT
public:
    explicit QSchemeEditor(QWidget* parent = nullptr);

    void addBlock(QBlock* block);

protected:
    void wheelEvent ( QWheelEvent * event ) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent* event) override;

    void updateSceneRect();

protected slots:
    void scalingTime(qreal x);
    void animFinished();

    void pinPressed(QPin* pin);
    void pinFocussed(QPin* pin, bool isFocussed);

private:
    SchemeEditorModel model;

    int numScheduledScalings;
    int origin_x;
    int origin_y;

    QPointF center;
    qreal scale;
};

#endif //BLOCK_ENGINE_QSCHEMEEDITOR_H
