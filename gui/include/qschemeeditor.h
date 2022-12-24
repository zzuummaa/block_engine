#ifndef BLOCK_ENGINE_QSCHEMEEDITOR_H
#define BLOCK_ENGINE_QSCHEMEEDITOR_H

#include <QGraphicsView>
#include <map>
#include <unordered_map>
#include "qbusline.h"
#include "qbus.h"
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
    void tryAddBusLine(QPointF from, QPointF to);
    void tryAddBusLine(QPin*, QPin*);
    void tryAddBusLine(QPin*, QPointF);

protected:
    void addBusLine(const std::shared_ptr<QBus>& bus, QPointF from, QPointF to);
    std::shared_ptr<QBus> concatBusses(const std::shared_ptr<QBus>& bus1, const std::shared_ptr<QBus>& bus2);
    std::pair<QGraphicsRectItem*, QGraphicsProxyWidget*> addSceneProxy(QWidget*, qreal x, qreal y);

    void wheelEvent ( QWheelEvent * event ) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

    void keyReleaseEvent(QKeyEvent* event) override;

    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

    void updateSceneRect();

protected slots:
    void scalingTime(qreal x);
    void animFinished();

    void pinPressed(QPin* pin);
    void pinFocussed(QPin* pin, QRectF pinRect);

private:
    constexpr static const qreal BUS_LINE_LENGTH_THRESHOLD = 1.0;

    std::map<QPin*, QGraphicsProxyWidget*> proxyByPin;
    std::map<QWidget*, std::shared_ptr<QBus>> busses;

    SchemeEditorModel model;
    QPinLinkDetector pinLinker;

    std::optional<QPoint> leftPressPos;

    bool isShiftPressed;

    int numScheduledScalings;
    int origin_x;
    int origin_y;

    QPointF center;
    qreal scale;
};

#endif //BLOCK_ENGINE_QSCHEMEEDITOR_H
