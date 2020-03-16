#ifndef FILETHREAD_H
#define FILETHREAD_H
#include <QObject>
#include <QThread>
#include <QtNetwork/QTcpSocket>
#include <QFile>
#include <QDataStream>
class FileThread : public QThread
{
    Q_OBJECT
public:
    QString msg_str;
    FileThread(QTcpSocket *clientsocket);
protected:
    void run();
private slots:
        void readClient();
signals:
        void signal_updateUI();
private:
    QTcpSocket * tcp_clientsocket;
    qint64 totalBytes =0;//存放总大小信息
    qint64 bytesReceived =0;//已收到数据大小
    qint64 fileNameSize =0;//文件名大小
    QFile *localFile;//保存进本地文件信息
    QString fileName;
    QByteArray inBlock;//数据缓冲区
};

#endif // FILETHREAD_H
