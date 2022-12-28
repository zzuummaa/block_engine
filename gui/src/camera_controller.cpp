#include "scheme_editor/camera_controller.h"

#include <qtimeline.h>

CameraController::CameraController(
    QGraphicsView* view,
    QObject* parent)
    : QObject(parent)
    , view(view)
    , scale(1.0)
    , numScheduledScalings(0) {
}


void CameraController::moveStartEvent(QPoint pos) {
    lastPos = pos;
}

void CameraController::moveEvent(QPoint pos) {
    auto diff = view->mapToScene(lastPos) - view->mapToScene(pos);
    center += diff;
    lastPos = pos;
    updateSceneRect();
}

void CameraController::zoomEvent(int numSteps) {
    numScheduledScalings += numSteps;
    if (numScheduledScalings * numSteps < 0) {
        numScheduledScalings = numSteps;
    }

    auto anim = new QTimeLine(350, this);
    anim->setUpdateInterval(20);

    connect(anim, SIGNAL (valueChanged(qreal)), SLOT (scalingTime(qreal)));
    connect(anim, SIGNAL (finished()), SLOT (animFinished()));
    anim->start();
}

QPointF CameraController::sceneCenter() {
    return center;
}

void CameraController::setSceneCenter(QPointF newCenter) {
    center = newCenter;
    updateSceneRect();
}

void CameraController::updateSceneRect() {
    const auto editorBounds = view->mapToScene(view->rect()).boundingRect();
    const auto padding = view->mapToScene(0, 0, 4, 4).boundingRect().size();

    view->scene()->setSceneRect(
        center.x() - editorBounds.width() / 2.0 + padding.width(),
        center.y() - editorBounds.height() / 2.0 + padding.height(),
        editorBounds.width() - padding.width() * 2,
        editorBounds.height() - padding.height() * 2);

//    qDebug() << "QSchemeEditor::updateSceneRect() bounds: " << editorBounds << ", center:" << editorBounds.center();
}

void CameraController::scalingTime(qreal) {
    qreal factor = 1.0 + qreal(numScheduledScalings) / 300.0;
    scale *= factor;

    auto transform = QTransform(scale, 0, 0, scale, center.x(), center.y());

    view->setTransform(transform);

    updateSceneRect();
}

void CameraController::animFinished() {
    if (numScheduledScalings > 0) {
        numScheduledScalings--;
    } else {
        numScheduledScalings++;
    }
    sender()->~QObject();
}
