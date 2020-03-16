#include "login_thread.h"
#include <QThread>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QStringList>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QMessageBox>
#include <mysql_operateor.h>
login_thread::login_thread(QTcpSocket *clientsocket)
{
    tcp_clientsocket = clientsocket;
}

void login_thread::run()
{
    connect(tcp_clientsocket,SIGNAL(readyRead()),this,SLOT(readClient()));
}

void login_thread::readClient()
{
    QByteArray msg_byte = tcp_clientsocket->readAll();
    QString number_password = msg_byte;
    QStringList list = number_password.split("#");
    int sql_number = list[0].toInt();
    //验证
    bool login_success = Verify(number_password);
    qDebug()<<login_success<<endl;
    //返回数据
    if(login_success)
    {
        MySql_operateor *update_sql = new MySql_operateor();
        update_sql->operator_sql("update",QString::number(sql_number));
        QString success_msg = "success";
        QByteArray successArray = success_msg.toUtf8();
        this->tcp_clientsocket->write(successArray);
        msg_str ="客户端" + name + "登录成功";
    }else{
        QString fail_msg = "fail";
        QByteArray failArray = fail_msg.toUtf8();
        this->tcp_clientsocket->write(failArray);
        msg_str ="客户端" + name + "登录失败";
    }
    emit signal_updateUI();
}

bool login_thread::Verify(QString number_passwords)
{
    //访问数据库
    /*QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db=QSqlDatabase::database("qt_sql_default_connection");
    }
    else{
        db = QSqlDatabase::addDatabase("QMYSQL");
    }
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
    qDebug()<<"进行数据库查询"<<endl;
    //数据库查询
    QSqlQuery query;
    query.exec("SELECT * FROM user");
    while(query.next())
    {
        QString numbers = query.value(0).toString();
        QString passwords = query.value(1).toString();
        QString number_password = numbers + "#" + passwords;
        name = query.value(2).toString();
        qDebug()<<name<<endl;
        if(number_password==number_passwords)
        {
          db.close();
          return true;
        }
    }
    db.close();
    return false;*/
    MySql_operateor *query = new MySql_operateor();
    bool query_result = query->operator_sql("query",number_passwords);
    return query_result;
}
