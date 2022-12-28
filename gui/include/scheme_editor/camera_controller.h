#pragma once

#include <QGraphicsView>

class CameraController : public QObject {
    Q_OBJECT
public:
    CameraController(QGraphicsView* view, QObject* parent = nullptr);

    void moveEvent(QPointF pos);
    void zoomEvent(int numSteps);

    QPointF sceneCenter();

    void updateSceneRect();

protected slots:
    void scalingTime(qreal x);
    void animFinished();

private:
    QGraphicsView* view;

    QPointF center;
    qreal scale;
    int numScheduledScalings;
};