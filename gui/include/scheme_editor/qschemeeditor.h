#pragma once

#include "qbusline.h"
#include "qbus.h"
#include "qblock.h"
#include "camera_controller.h"
#include "scene_controller.h"
#include "scheme_editor_model.h"

#include <map>
#include <unordered_map>

#include <QGraphicsView>

class QSchemeEditor : public QGraphicsView  {
    Q_OBJECT
public:
    SchemeEditorModel model;
    CameraController* cameraController;
    SceneController* sceneController;

    explicit QSchemeEditor(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
    void keyReleaseEvent(QKeyEvent* event) Q_DECL_OVERRIDE;

    void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

private:
    QPoint lastPos;
};
