#ifndef BLOCK_ENGINE_QSCHEMEEDITOR_H
#define BLOCK_ENGINE_QSCHEMEEDITOR_H

#include <QWidget>

class QSchemeEditor : public QWidget {
    Q_OBJECT
public:
    explicit QSchemeEditor(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
};

#endif //BLOCK_ENGINE_QSCHEMEEDITOR_H
