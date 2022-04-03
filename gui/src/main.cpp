#include <QApplication>
#include "qblockenginewindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QBlockEngineWindow window;
    window.show();

    return app.exec();
}
