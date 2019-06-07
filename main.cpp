#include "bridgegame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BridgeGame w;
    w.show();
    return a.exec();
}
