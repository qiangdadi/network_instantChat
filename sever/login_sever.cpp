#include "login_sever.h"
#include <QtNetwork>
#include "filethread.h"
#include <QDebug>
login_sever::login_sever(QObject *parent) : QObject(parent)
{

}

void login_sever::start_loginserver()
{
    login_server = new QTcpServer();
    //对于IPV4，IPV6地址进行监听,端口6664
    login_server->listen(QHostAddress::Any,6664);
    qDebug()<<"开始监听6664端口"<<endl;
    connect(login_server,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
}
void login_sever::acceptConnection()
{
    QString tcp_name;
    tcp_name="f"+QString::number(id);
    qDebug()<<"tcp_name"<<tcp_name<<"\n";
    id++;
    QTcpSocket *clientConnection = login_server->nextPendingConnection();
    clientConnection->setObjectName(tcp_name);
    connect(clientConnection,SIGNAL(disconnected()),this,SLOT(delete_tcp()));
    clientConnectionList.append(clientConnection);
    login_thread *thread = new login_thread(clientConnection);
    thread->setObjectName(tcp_name);
    qDebug()<<"thread_name"<<tcp_name<<"\n";
    connect(thread,SIGNAL(signal_updateUI()),this,SLOT(update_UI()));
    thread->start();
    threadList.append(thread);
}
void login_sever::delete_tcp()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString tcp_name = client->objectName();
    //删除Thread对应的线程
    login_thread *thread;
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
    login_msg = "客户端断开成功" + tcp_name +"\n";
    emit login_update_UI();
}
void login_sever::update_UI()
{
    login_thread *thread = qobject_cast<login_thread *>(sender());
    login_msg= thread->msg_str + "\n";
    emit login_update_UI();
}

