#include "qschemeeditor.h"
#include <qgraphicsitem.h>

QSchemeEditor::QSchemeEditor(QWidget* parent) : QGraphicsView(parent) {
    auto scene = new QGraphicsScene(this);

    setScene(scene);

    QBrush red_brush(Qt::red);

    auto rect = scene->addRect(0, 0, 50, 50, QPen(), red_brush);
    rect->setFlag(QGraphicsItem::ItemIsMovable);
}
