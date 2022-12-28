#include <qgraphicsitem.h>
#include <qgraphicsproxywidget.h>
#include <qdebug.h>
#include <QTimeLine>
#include <qscrollbar.h>
#include <variant>
#include <cmath>

#include "scheme_editor/qschemeeditor.h"

using namespace block_engine;

QSchemeEditor::QSchemeEditor(QWidget* parent) : QGraphicsView(parent) {

    auto scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(QBrush(Qt::gray));
//    scene->setSceneRect(-50000, -50000, 100000, 100000);
//    scene->addEllipse(-500, -500, 1000, 1000, QPen(Qt::black), QBrush(Qt::NoBrush));

    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setScene(scene);

    cameraController = new CameraController(this, this);
    sceneController = new SceneController(&model, scene, this);
}

void QSchemeEditor::resizeEvent(QResizeEvent* event) {
    cameraController->updateSceneRect();
    QGraphicsView::resizeEvent(event);
}

void QSchemeEditor::wheelEvent(QWheelEvent* event) {
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // see QWheelEvent documentation
    cameraController->zoomEvent(numSteps);
}

// =========================== Scene moving ===========================

void QSchemeEditor::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        sceneController->mousePressEvent(event->button(), mapToScene(event->pos()), itemAt(event->pos()));
    } else if (event->button() == Qt::RightButton) {
        lastPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void QSchemeEditor::mouseMoveEvent(QMouseEvent* event) {
    if (event->buttons() & Qt::RightButton) {
        auto diff = mapToScene(lastPos) - mapToScene(event->pos());
        cameraController->moveEvent(cameraController->sceneCenter() + diff);
        lastPos = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

// =========================== Bus creating ===========================

void QSchemeEditor::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton) {
        sceneController->mouseReleaseEvent(event->button(), mapToScene(event->pos()), itemAt(event->pos()));
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void QSchemeEditor::keyPressEvent(QKeyEvent* event) {
    sceneController->keyPressEvent(event->key());
    QGraphicsView::keyPressEvent(event);
}

void QSchemeEditor::keyReleaseEvent(QKeyEvent* event) {
    sceneController->keyReleaseEvent(event->key());
    QGraphicsView::keyReleaseEvent(event);
}
