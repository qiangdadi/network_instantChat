#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include <QString>
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::on_connect_clicked()
{
    client = new QTcpSocket;
    client->connectToHost(QHostAddress::LocalHost,6665);
    QString empty_msg = user_number + "#"+"0"+"#"+" ";//发送空消息;
    qDebug()<<user_number<<endl;
    QByteArray numberArray = empty_msg.toUtf8();
    client->write(numberArray);
    receive_entity = new Receive_Thread(client);
    receive_thread = new QThread();
    receive_entity->moveToThread(receive_thread);
    connect(receive_entity,SIGNAL(signal_msgCome(QString,int)),this,SLOT(show_msg(QString,int)));
    connect(receive_thread,SIGNAL(started()),receive_entity,SLOT(ready_read()));
    receive_thread->start();
    qDebug()<<"线程启动"<<endl;
}

void LoginWindow::on_send_clicked()
{
    QString str = ui->msg->toPlainText().trimmed();
    if(str==""){
        QMessageBox::information(NULL,"tip","消息不能为空",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }else{
        QString msg = user_number + "#"+"205732204"+"#"+str;
        QByteArray msg_array = msg.toUtf8();
        client->write(msg_array);
    }
    ui->msg->setText("");
    ui->showmsg->append(str);
}

void LoginWindow::show_msg(QString msg, int type)
{
    qDebug()<<"更新消息"<<endl;
    QString show = "接收到消息： "+msg;
    ui->showmsg->append(show);
}
