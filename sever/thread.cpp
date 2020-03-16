#include "thread.h"
#include <QThread>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QStringList>
#include <QDateTime>
#include <QMessageBox>
#include <mysql_operateor.h>
#include "global.h"
Thread::Thread(QTcpSocket *clientsocket)
{
    tcp_clientsocket = clientsocket;
}

void Thread::run()
{
    connect(tcp_clientsocket,SIGNAL(readyRead()),this,SLOT(readClient()));
}

void Thread::readClient()
{
    QByteArray msg_byte = tcp_clientsocket->readAll();
    QString msg = msg_byte;
    QStringList list = msg.split("#");
    int sql_sender = list[0].toInt();
    int sql_receiver = list[1].toInt();
    QString sql_content = list[2];
    //第一次建立tcp连接时接收从客户端发来的用于指定tcp连接的名字
    if(sql_receiver==0)
    {
        QString tcp_name=QString::number(sql_sender)+ "#"+tcp_clientsocket->objectName() ;
        //qDebug()<<tcp_name<<endl;
        name=tcp_name;
        emit signal_changeName();
    }else{
        save_Msg(msg);
        QTcpSocket *tcp_socket;
        bool find=false;
        //如果receiver在线直接发送
        for(int j=0;j<clientConnectionList.length();j++){
            qDebug()<<clientConnectionList.at(j)->objectName()<<QString::number(sql_receiver)<<endl;
            if(clientConnectionList.at(j)->objectName()==QString::number(sql_receiver)){
                tcp_socket=clientConnectionList.at(j);
                find=true;
                break;
            }
        }
        //如果有对方的tcp连接即对方在线
        if(find){
            //服务器转发给客户端
            tcp_socket->write(sql_content.toUtf8());
            find=false;
        }else{
            QMessageBox::information(NULL, "信息提示", "对方不在线.",
                                     QMessageBox::Yes | QMessageBox::No,
                                     QMessageBox::Yes);
        }
        msg_str ="客户端" + tcp_clientsocket->objectName() +":  "+ msg;
        emit signal_updateUI();
    }
}

void Thread::save_Msg(QString msg)
{
    //拆分数据
    /*QStringList list = msg.split("#");
    int sql_sender = list[0].toInt();
    int sql_receiver = list[1].toInt();
    QString sql_content = list[2];
    int sql_content_type = 0;//文字消息
    QDateTime sql_time = QDateTime::currentDateTime();
    //访问数据库
    QSqlDatabase db;

    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");//数据库主机名
    db.setDatabaseName("network_cs_app");//数据库名
    db.setUserName("root");//数据库用户名
    db.setPassword("");//数据库密码
    if(!db.open()){
        QMessageBox::information(NULL, "信息提示", "数据库连接失败.",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
        db.close();
        exit(EXIT_FAILURE);
    }
    QSqlQuery query;
    query.prepare("INSERT INTO MESSAGE (sender,receiver,content,content_type,time)" "VALUES(:sender,:receiver,:content,:content_type,:time)");
    query.bindValue(":sender",sql_sender);
    query.bindValue(":receiver",sql_receiver);
    query.bindValue(":content",sql_content);
    query.bindValue(":content_type",sql_content_type);
    query.bindValue(":time",sql_time);
    query.exec();
    qDebug()<<"插入数据成功"<<sql_content<<endl;
    db.close();*/
    MySql_operateor *insert = new MySql_operateor();
    insert->operator_sql("insert",msg);

}
