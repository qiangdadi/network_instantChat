#include "sever.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Sever w;
    w.show();

    return a.exec();
}
