#ifndef LOGIN_THREAD_H
#define LOGIN_THREAD_H
#include <QThread>
#include <QTcpSocket>
#include <QObject>
#include <QString>
#include <QFile>
#include <QByteArray>

class login_thread : public QThread
{
    Q_OBJECT
public:
    QString msg_str;
    login_thread(QTcpSocket *clientsocket);
protected:
    void run();
private slots:
        void readClient();
signals:
        void signal_updateUI();
private:
    QString qq_number;
    QString password;
    QString name;
    QTcpSocket * tcp_clientsocket;
    bool Verify(QString number_passwords);
};

#endif // LOGIN_THREAD_H
