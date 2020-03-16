#ifndef RECEIVE_THREAD_H
#define RECEIVE_THREAD_H
#include <QTcpSocket>
#include <QString>

class Receive_Thread :public QObject
{
    Q_OBJECT
public:
    QString msg;
    Receive_Thread(QTcpSocket *client_socket);
signals:
    void signal_msgCome(QString msg,int type);
private slots:
    void readServer();
    void ready_read();
private:
    QTcpSocket *tcp_clientsocket;


};

#endif // RECEIVE_THREAD_H
