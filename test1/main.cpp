#include "widget.h"
#include <QApplication>
#include "loginwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginWindow w;
    /*if(!w.Auto_Login){
      w.show();
    }*/
    w.show();
    return a.exec();
}
