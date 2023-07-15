#include "computerinfo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ComputerInfo w;
    w.show();
    return a.exec();
}
