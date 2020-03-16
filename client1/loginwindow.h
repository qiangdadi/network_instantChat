#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include <QtNetwork/QTcpSocket>
#include <QMainWindow>
#include <QThread>
#include "receive_thread.h"
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_connect_clicked();

    void on_send_clicked();
    void show_msg(QString msg,int type);

private:
    QTcpSocket *client;
    Ui::LoginWindow *ui;
    QString user_number = "205731204";
    QThread *receive_thread;
    Receive_Thread *receive_entity;
};

#endif // LOGINWINDOW_H
