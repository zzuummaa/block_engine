#pragma once

#include "qbus.h"
#include "qblock.h"
#include "scheme_editor_model.h"

#include <qgraphicsitem.h>

class SceneController : public QObject {
    Q_OBJECT
public:
    SceneController(SchemeEditorModel* model, QGraphicsScene* scene, QObject* parent = nullptr);

    void addBlock(QBlock* block, QPointF pos);
    void tryAddBusLine(QPointF from, QPointF to);
    void tryAddBusLine(QPin*, QPin*);
    void tryAddBusLine(QPin*, QPointF);

    void mousePressEvent(Qt::MouseButton button, QPointF pos, QGraphicsItem* item = nullptr);
    void mouseReleaseEvent(Qt::MouseButton button, QPointF pos, QGraphicsItem* item = nullptr);

    void keyPressEvent(int key);
    void keyReleaseEvent(int key);

protected:
    void addBusLine(const std::shared_ptr<QBus>& bus, QPointF from, QPointF to);
    std::shared_ptr<QBus> concatBusses(const std::shared_ptr<QBus>& bus1, const std::shared_ptr<QBus>& bus2);
    std::pair<QGraphicsRectItem*, QGraphicsProxyWidget*> addSceneProxy(QWidget*, QPointF);

private:
    SchemeEditorModel* model;
    QGraphicsScene* scene;

    std::map<QPin*, QGraphicsProxyWidget*> proxyByPin;
    std::map<QWidget*, std::shared_ptr<QBus>> busses;

    std::optional<QPointF> leftPressPos;
    QGraphicsItem* leftPressItem;

    bool isShiftPressed;
};