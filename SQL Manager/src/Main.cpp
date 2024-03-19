#include "SQLManagerGUI.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQLManagerGUI w;
    w.show();
    return a.exec();
}
