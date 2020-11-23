#include "sccp.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    sccp w;
    w.show();
    return a.exec();
}
