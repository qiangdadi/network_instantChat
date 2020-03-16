#include "fileserver.h"
#include <QtNetwork>
#include "filethread.h"
FileServer::FileServer(QObject *parent) : QObject(parent)
{

}

void FileServer::start_fileserver()
{
    file_server = new QTcpServer();
    //对于IPV4，IPV6地址进行监听,端口6666
    file_server->listen(QHostAddress::Any,6666);
    connect(file_server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}
void FileServer::acceptConnection()
{
    QString tcp_name;
    tcp_name="f"+QString::number(id);
    qDebug()<<"tcp_name"<<tcp_name<<"\n";
    id++;
    QTcpSocket *clientConnection = file_server->nextPendingConnection();
    clientConnection->setObjectName(tcp_name);
    connect(clientConnection,SIGNAL(disconnected()),this,SLOT(delete_tcp()));
    clientConnectionList.append(clientConnection);
    FileThread *thread = new FileThread(clientConnection);
    thread->setObjectName(tcp_name);
    qDebug()<<"thread_name"<<tcp_name<<"\n";
    connect(thread,SIGNAL(signal_updateUI()),this,SLOT(update_UI()));
    thread->start();
    threadList.append(thread);
}
void FileServer::delete_tcp()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString tcp_name = client->objectName();
    //删除Thread对应的线程
    FileThread *thread;
    int i;
    for(i=0;i<threadList.length();i++){
        if(threadList.at(i)->objectName() == tcp_name){
            thread = threadList.at(i);
            break;
        }
    }
    //disconnect(thread,SIGNAL(signal_updateUI()),this,SLOT(update_UI()));
    thread->quit();
    thread->wait();
    //delete thread;
    threadList.removeAt(i);
    //删除对应的socket
    int j;
    QTcpSocket *tcpsocket;
    for(j=0;j<clientConnectionList.length();j++){
        if(clientConnectionList.at(j)->objectName()==tcp_name){
            tcpsocket = clientConnectionList.at(j);
            break;
        }
    }
    //disconnect(tcpsocket,SIGNAL(disconnected()),this,SLOT(delete_tcp()));
    clientConnectionList.removeAt(j);
    file_msg = "客户端断开成功" + tcp_name +"\n";
    emit file_update_UI();
}
void FileServer::update_UI()
{
    FileThread *thread = qobject_cast<FileThread *>(sender());
    file_msg= thread->msg_str + "\n";
    emit file_update_UI();
}
