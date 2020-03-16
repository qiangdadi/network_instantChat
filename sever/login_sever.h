#ifndef LOGIN_SEVER_H
#define LOGIN_SEVER_H
#include <QObject>
#include <QtNetwork>
#include "login_thread.h"
#include <QString>

class login_sever : public QObject
{
    Q_OBJECT
public:
    QString login_msg;
    explicit login_sever(QObject *parent = nullptr);

signals:
    void login_update_UI();
public slots:
    void acceptConnection();
    void start_loginserver();
    void delete_tcp();
    void update_UI();
private:
    int id=0;
    QTcpServer *login_server;
    QList<QTcpSocket *> clientConnectionList;
    QList<login_thread *> threadList;
};

#endif // LOGIN_SEVER_H
