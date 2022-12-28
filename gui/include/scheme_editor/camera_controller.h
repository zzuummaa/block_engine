#pragma once

#include <QGraphicsView>

class CameraController : public QObject {
    Q_OBJECT
public:
    CameraController(QGraphicsView* view, QObject* parent = nullptr);

    void moveStartEvent(QPoint pos);
    void moveEvent(QPoint pos);
    void zoomEvent(int numSteps);

    QPointF sceneCenter();
    void setSceneCenter(QPointF newCenter);

    void updateSceneRect();

protected slots:
    void scalingTime(qreal x);
    void animFinished();

private:
    QGraphicsView* view;

    QPoint lastPos;

    QPointF center;
    qreal scale;
    int numScheduledScalings;
};