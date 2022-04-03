#include <QApplication>
#include "blockenginewindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    BlockEngineWindow window;
    window.show();

    return app.exec();
}
