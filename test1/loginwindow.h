#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include "widget.h"
#include <QtNetwork/QTcpSocket>
namespace Ui {
class LoginWindow;
}

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    bool Auto_Login;
    Widget * main_interface;
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_logi_button_clicked();

    void on_rem_password_clicked(bool checked);

    void on_auto_login_clicked(bool checked);

    void readMessage();

private:
    QTcpSocket *login_socket;
    //QTcpSocket *tcp_client;
    Ui::LoginWindow *ui;
    bool if_login=false;
    void login();
    void sendMessage(QString msg);
};

#endif // LOGINWINDOW_H
