#ifndef FILESERVER_H
#define FILESERVER_H

#include <QObject>
#include <QtNetwork>
#include "filethread.h"
#include <QString>
class FileServer : public QObject
{
    Q_OBJECT
public:
    QString file_msg;
    explicit FileServer(QObject *parent = nullptr);

signals:
    void file_update_UI();
public slots:
    void acceptConnection();
    void start_fileserver();
    void delete_tcp();
    void update_UI();
private:
    int id=0;
    QTcpServer *file_server;
    QList<QTcpSocket *> clientConnectionList;
    QList<FileThread *> threadList;
};

#endif // FILESERVER_H
