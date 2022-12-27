#include <QApplication>
#include <qtextcodec.h>
#include "qblockenginewindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    setlocale(LC_ALL, "");

    QBlockEngineWindow window;
    window.show();

    return app.exec();
}
