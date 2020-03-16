#ifndef SEVER_H
#define SEVER_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QList>
#include <QThread>
#include "thread.h"
#include "filethread.h"
#include "fileserver.h"
#include "login_sever.h"
#include "login_thread.h"
namespace Ui {
class Sever;
}

class Sever : public QMainWindow
{
    Q_OBJECT

public:
    explicit Sever(QWidget *parent = 0);
    ~Sever();

private slots:
    void on_startSever_clicked();
    void acceptConnection();
    void delete_tcp();
    void update_UI();
    void f_update_UI();
    void login_update_UI();
    void receiveTcpName();//更新tcpname.
protected:

private:
    int id=0;
    bool isclicked = false;
    QTcpServer *sever;
    FileServer *fileserver;
    login_sever *login_Sever;
    QThread *fileThread;
    QThread *login_Thread;

    QList<Thread *> threadList;
    Ui::Sever *ui;
};

#endif // SEVER_H
