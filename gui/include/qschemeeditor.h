#ifndef BLOCK_ENGINE_QSCHEMEEDITOR_H
#define BLOCK_ENGINE_QSCHEMEEDITOR_H

#include <QGraphicsView>
#include "qbusline.h"
#include "qblock.h"
#include "scheme_editor_model.h"

class QPinLinkDetector : public QObject {
Q_OBJECT
private:
    enum class State {
        Ready,
        PinPressed,
        MouseReleased
    };

public:
    QPinLinkDetector();

    void pinPressed(QPin* pin);
    void mouseReleased(QPoint pos);
    void mouseMoved(QPin* pin, QRectF pinRect);

    void reset();

signals:
    void link(QPin* from, QPin* to);

private:
    State state;
    QPin* pressedPin;
    QPoint releasePosition;
};

class QSchemeEditor : public QGraphicsView  {
    Q_OBJECT
public:
    explicit QSchemeEditor(QWidget* parent = nullptr);

    void addBlock(QBlock* block);
    void addBusLine(QBusLine* busLine);

protected:
    void wheelEvent ( QWheelEvent * event ) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void resizeEvent(QResizeEvent* event) override;

    void updateSceneRect();

protected slots:
    void scalingTime(qreal x);
    void animFinished();

    void pinPressed(QPin* pin);
    void pinFocussed(QPin* pin, QRectF pinRect);

private:
    std::map<QPin*, QGraphicsProxyWidget*> proxyByPin;

    SchemeEditorModel model;
    QPinLinkDetector pinLinker;

    int numScheduledScalings;
    int origin_x;
    int origin_y;

    QPointF center;
    qreal scale;
};

#endif //BLOCK_ENGINE_QSCHEMEEDITOR_H
