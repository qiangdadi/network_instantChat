#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QFile>
#include <QByteArray>
#include <QList>
class Thread : public QThread
{
    Q_OBJECT
public:
    QString msg_str;
    QString name;
    Thread(QTcpSocket *clientsocket);
protected:
    void run();
private slots:
        void readClient();

signals:
        void signal_updateUI();
        void signal_changeName();
private:
    //QList<QTcpSocket *> tcp_clientConnectionList;
    QTcpSocket * tcp_clientsocket;
    void save_Msg(QString msg);
};

#endif // THREAD_H
