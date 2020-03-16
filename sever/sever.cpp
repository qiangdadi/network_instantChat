#include "sever.h"
#include "ui_sever.h"
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QThread>
#include "thread.h"
#include "filethread.h"
#include "fileserver.h"
#include "login_sever.h"
#include "login_thread.h"
#include <QDebug>
#include <QMetaType>
#include "global.h"
Sever::Sever(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Sever)
{
    ui->setupUi(this);
}

Sever::~Sever()
{
    delete ui;
}

void Sever::on_startSever_clicked()
{
    if(!isclicked){
        isclicked = true;
        sever = new QTcpServer();
        //消息服务器
        //对于IPV4，IPV6地址进行监听,端口6665
        sever->listen(QHostAddress::Any,6665);
        connect(sever,SIGNAL(newConnection()),this,SLOT(acceptConnection()));
        //开启文件服务器
        fileserver = new FileServer();
        fileThread = new QThread();
        fileserver->moveToThread(fileThread);
        connect(fileserver,SIGNAL(file_update_UI()),this,SLOT(f_update_UI()));
        connect(fileThread,SIGNAL(started()),fileserver,SLOT(start_fileserver()));
        fileThread->start();
        //开启登录服务器
        login_Sever = new login_sever();
        login_Thread = new QThread();
        login_Sever->moveToThread(login_Thread);
        connect(login_Sever,SIGNAL(login_update_UI()),this,SLOT(login_update_UI()));
        connect(login_Thread,SIGNAL(started()),login_Sever,SLOT(start_loginserver()));
        login_Thread->start();

        ui->msg_Show->setText("服务端启动\n");
        ui->startSever->setText("关闭服务器");
    }else{
        sever->close();
        this->close();
    }
}

void Sever::acceptConnection()
{
    ui->msg_Show->append("客户端连接上\n");
    QString tcp_name;
    tcp_name="m"+QString::number(id);
    //qDebug()<<"tcp_name"<<tcp_name<<"\n";
    id++;
    QTcpSocket *clientConnection = sever->nextPendingConnection();
    clientConnection->setObjectName(tcp_name);
    connect(clientConnection,SIGNAL(disconnected()),this,SLOT(delete_tcp()));
    clientConnectionList.append(clientConnection);
    Thread *thread = new Thread(clientConnection);
    thread->setObjectName(tcp_name);
    //qDebug()<<"thread_name"<<tcp_name<<"\n";
    connect(thread,SIGNAL(signal_updateUI()),this,SLOT(update_UI()));
    connect(thread,SIGNAL(signal_changeName()),this,SLOT(receiveTcpName()));
    thread->start();
    threadList.append(thread);
}
void Sever::delete_tcp()
{
    QTcpSocket *client = qobject_cast<QTcpSocket *>(sender());
    QString tcp_name = client->objectName();
    //删除Thread对应的线程
    Thread *thread;
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
    ui->msg_Show->append("客户端断开成功:"+tcp_name+"\n");
}

void Sever::update_UI()
{
    Thread *thread = qobject_cast<Thread *>(sender());
    QString msg = thread->msg_str + "\n";
    ui->msg_Show->append(msg);
}
void Sever::f_update_UI()
{
    FileServer *fileserver = qobject_cast<FileServer *>(sender());
    QString msg = fileserver->file_msg;
    ui->msg_Show->append(msg);
}

void Sever::login_update_UI()
{
    login_sever *Login_Sever = qobject_cast<login_sever*>(sender());
    QString msg = Login_Sever->login_msg;
    ui->msg_Show->append(msg);
}

void Sever::receiveTcpName()
{
    //qDebug()<<"触发改名字信号"<<endl;
    Thread *thread = qobject_cast<Thread *>(sender());
    QString name = thread->name;
    QStringList list = name.split("#");
    QString sql_sender = list[0];
    QString tcp_name = list[1];
    //qDebug()<<tcp_name<<endl;
    for(int j=0;j<clientConnectionList.length();j++){
        //qDebug()<<clientConnectionList.at(j)->objectName()<<endl;
        if(clientConnectionList.at(j)->objectName()==tcp_name){
            clientConnectionList.at(j)->setObjectName(sql_sender);
            //qDebug()<<"tcpname修改成功:"<<tcp_name<<endl;
            break;
        }
    }
    for(int i=0;i<threadList.length();i++){
        if(threadList.at(i)->objectName() == tcp_name){
            threadList.at(i)->setObjectName(sql_sender);
            break;
        }
    }
}
