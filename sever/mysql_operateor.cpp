#include "mysql_operateor.h"
#include <QSql>
#include <QMessageBox>
#include <QDateTime>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
int MySql_operateor::i=0;
MySql_operateor::MySql_operateor()
{

}

bool MySql_operateor::operator_sql(QString type, QString msg)
{
    int flag =0;
    QString connectionName;
    connectionName="MySql_"+QString::number(i);
    {
    QSqlDatabase db = openDataBase(connectionName);
    QSqlQuery query(db);
    if(type=="insert")
    {
        QStringList list = msg.split("#");
        int sql_sender = list[0].toInt();
        int sql_receiver = list[1].toInt();
        QString sql_content = list[2];
        int sql_content_type = 0;//文字消息
        QDateTime sql_time = QDateTime::currentDateTime();
        QString sql_time_str = sql_time.toString("yyyy-MM-dd hh:mm:ss");

        query.prepare("INSERT INTO MESSAGE(sender,receiver,content,content_type,time)"
                      "VALUES(?,?,?,?,?);");
        query.addBindValue(sql_sender);
        query.addBindValue(sql_receiver);
        query.addBindValue(sql_content);
        query.addBindValue(sql_content_type);
        query.addBindValue(sql_time_str);
        /*QString head = "INSERT INTO MESSAGE(sender,receiver,content,content_type,time) VALUES (";
        QString middle = ",";
        QString end = ")";
        QString sentence = head +sql_sender+middle+sql_receiver+middle+sql_content+middle+sql_content_type+middle+sql_time_str+end;*/
        if(query.exec())
        {
            qDebug()<<"插入数据成功"<<sql_time_str<<endl;
        }else
        {
            qDebug()<<"query error:"<<query.lastError();
        }
        db.close();
    }
    if(type=="query")
    {
        query.exec("SELECT * FROM user");
        while(query.next())
        {
            QString numbers = query.value(0).toString();
            QString passwords = query.value(1).toString();
            QString number_password = numbers + "#" + passwords;
            QString name = query.value(2).toString();
            qDebug()<<name<<endl;
            if(number_password==msg)
            {
              flag = 1;
              break;
            }
        }
        db.close();
    }
    if(type=="delete")
    {

    }
    if(type=="update")
    {
        query.prepare("UPDATE USER SET is_login=? WHERE user_number=?");
        query.addBindValue("1");
        query.addBindValue(msg);
        if(!query.exec())
        {
            qDebug()<<"update Error:"<<query.lastError();
        }
    }
    }
    closeDataBase(connectionName);
    if(flag==1)
    {
        return true;
    }
    return false;
}

QSqlDatabase MySql_operateor::openDataBase(QString connectionName)
{
    QSqlDatabase db;
    if(QSqlDatabase::contains(connectionName)==true)
    {
        db=QSqlDatabase::database(connectionName);
    }
    else
    {
        db=QSqlDatabase::addDatabase("QMYSQL",connectionName);
        db.setHostName("localhost");//数据库主机名
        db.setDatabaseName("network_cs_app");//数据库名
        db.setUserName("root");//数据库用户名
        db.setPassword("");//数据库密码
    }
    if(!db.open()){
        QMessageBox::information(NULL, "信息提示", "数据库连接失败.",
                                 QMessageBox::Yes | QMessageBox::No,
                                 QMessageBox::Yes);
        db.close();
        exit(EXIT_FAILURE);
    }
    i++;
    return db;
}

void MySql_operateor::closeDataBase(QString connectionName)
{
    QSqlDatabase::removeDatabase(connectionName);
}
/*
void MySql_operateor::insert_sql(QString msg)
{

}

bool MySql_operateor::query_sql(QString sentence)
{
    QString connectionName;
    connectionName="MySql_"+QString::number(i);
    QSqlDatabase db = openDataBase(connectionName);
    QSqlQuery query(db);
    query.exec(sentence);
    while(query.next())
    {
        QString numbers = query.value(0).toString();
        QString passwords = query.value(1).toString();
        QString number_password = numbers + "#" + passwords;
        QString name = query.value(2).toString();
        qDebug()<<name<<endl;
        if(number_password==number_passwords)
        {
          closeDataBase(connectionName);
          return true;
        }
    }
    closeDataBase(connectionName);
    return false;
}

void MySql_operateor::delete_sql(QString sentence)
{

}

void MySql_operateor::update_sql(QString sentence)
{

}*/
