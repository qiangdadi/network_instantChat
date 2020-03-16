#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "widget.h"
#include <QString>
#include <QApplication>
#include <QMouseEvent>
#include <QEvent>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>
#include "global.h"
#include <QDebug>
LoginWindow::LoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
    //保存密码,自动登录
    QString str = "password";
    QDir dir;
    if(dir.exists(str))
    {
        QFile file("password/password.txt");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,"错误","文件打开失败，密码获取失败","确定");
            return;
        }
        QTextStream textInput(&file);
        QString number_password;
        number_password = textInput.readLine().trimmed();
        if(number_password != "")
        {
            QStringList list = number_password.split(" ");
            QString number = list[0];
            QString password = list[1];
            QString auto_login = list[2];
            if(auto_login == "0")
            {
                ui->qq_number->setText(number);
                ui->password->setText(password);
                Auto_Login = false;
            }else{
                ui->qq_number->setText(number);
                ui->password->setText(password);
                Auto_Login = true;
                login();
            }
        }
    }
    //setWindowIcon(QIcon(":/new/icon/image/icon"));
}
void LoginWindow::login()
{
    QString qq_number;
    QString password;
    qq_number = ui->qq_number->text().trimmed();
    password = ui->password->text().trimmed();
    QString msg = qq_number + "#" +password;
    //连接服务器
    qDebug()<<"连接服务器"<<endl;
    login_socket = new QTcpSocket();
    login_socket->connectToHost(QHostAddress::LocalHost,6664);
    if(!login_socket->ConnectedState){
         ui->tip_label->setText("连接服务器错误，请检查防火墙设置");
    }
   connect(this->login_socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
   //qDebug()<<"连接完成"<<endl;
   user_number = qq_number;
   //将账号密码发到服务器
   sendMessage(msg);
}
void LoginWindow::sendMessage(QString msg)
{
    qDebug()<<"发送消息"<<endl;
    QByteArray numberArray = msg.toUtf8();
    this->login_socket->write(numberArray);
}
//读取服务器发来的信息
void LoginWindow::readMessage()
{
    qDebug()<<"接收消息"<<endl;
    QByteArray qba = this->login_socket->readAll();
    QString msg = qba;
    if(msg =="success"){
        if_login=true;
    }else{
        if_login=false;
    }
    if(if_login)
    {
        this->close();
        //tcp_client->connectToHost(QHostAddress::LocalHost,6665);
        main_interface = new Widget();
        //QMouseEvent * clicked_xiaoxi = new QMouseEvent(QEvent::MouseButtonPress, QPoint(0, 0), Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);
        //QApplication::sendEvent(main_interface->ui->xiaoxi,clicked_xiaoxi);
        main_interface->ui->xiaoxi->clicked();
        main_interface->show();
    }else{
        ui->tip_label->setText("账号或密码错误，请重试");
    }
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_logi_button_clicked()
{
    login();
}

void LoginWindow::on_rem_password_clicked(bool checked)
{
    bool auto_login = false;
    if(checked)
    {
        QString str = "password\password.txt";
        QFileInfo fileinfo(str);
        if(!fileinfo.isFile()){
            QDir dir;
            dir.mkpath("password");
        }
        QFile file("password/password.txt");
        if(! file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存","确定");
            return;
        }
        QTextStream out(&file);
        out<<ui->qq_number->text().trimmed()<<" "<<ui->password->text().trimmed()<<" "<<QString::number(auto_login);
        file.close();
    }
}

void LoginWindow::on_auto_login_clicked(bool checked)
{
    bool auto_login = true;
    if(checked)
    {
        QString str = "password\password.txt";
        QFileInfo fileinfo(str);
        if(!fileinfo.isFile()){
            QDir dir;
            dir.mkpath("password");
        }
        QFile file("password/password.txt");
        if(! file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::critical(this,"错误","文件打开失败，信息没有保存","确定");
            return;
        }
        QTextStream out(&file);
        out<<ui->qq_number->text().trimmed()<<" "<<ui->password->text().trimmed()<<" "<<QString::number(auto_login);
        file.close();
    }
}
