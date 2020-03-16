#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>
#include <QTextDocumentFragment>
#include <QUrl>
#include <QTextBrowser>
#include <QPalette>
#include <QMessageBox>
#include <QByteArray>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QtNetwork>
#include <QTextDocument>
#include "global.h"
#include "receive_thread.h"
#include <QThread>
chatwindow::chatwindow(QString chatPerson,int p_sender,int receiver,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatwindow)
{
    ui->setupUi(this);
    setWindowTitle(chatPerson);
    this->p_sender=p_sender;
    this->receiver=receiver;
    //开启接收线程
    receive_entity = new Receive_Thread(tcp_client);
    receive_thread = new QThread();
    receive_entity->moveToThread(receive_thread);
    connect(receive_entity,SIGNAL(signal_msgCome(QString,int)),this,SLOT(show_msg(QString,int)));
    connect(receive_thread,SIGNAL(started()),receive_entity,SLOT(ready_read()));
    receive_thread->start();
    //qlistview_Adapter();
    //setWindowIcon(QIcon(":/new/icon/image/icon"));
}

chatwindow::~chatwindow()
{
    delete ui;
}
void chatwindow::qlistview_Adapter()
{
    /*
   for(int i=0;i<10;i++)
   {
       QListWidgetItem * item = new QListWidgetItem(ui->chatcontent,0);
       QWidget *w = new QWidget(ui->chatcontent);
       QHBoxLayout * layout = new QHBoxLayout(w);
       QString file = "G:/QT workspace/test1/image/head_icon.jpg";
       QPixmap pixmapa(file);
       QPixmap pixmap(40,40);
       pixmap.fill(Qt::transparent);
       QPainter painter(&pixmap);
       painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
       QPainterPath path;
       path.addEllipse(1,1,40,40);
       painter.setClipPath(path);
       painter.drawPixmap(1,1,40,40,pixmapa);
       QLabel *head_icon_label = new QLabel(w);
       head_icon_label->setPixmap(pixmap);
       QTextEdit * show_xiaoxi = new QTextEdit(w);
       //设置文本只读，居中显示
       show_xiaoxi->setReadOnly(true);
       show_xiaoxi->setAlignment(Qt::AlignVCenter);
       //设置QTextEdit根据内容调整大小
       connect(show_xiaoxi->document(),SIGNAL(contentsChanged()),this,SLOT(textAreaChanged()));
       show_xiaoxi->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       show_xiaoxi->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       show_xiaoxi->setStyleSheet("QTextEdit { background: blue }");
       if(i%2==0){
           show_xiaoxi->setHtml("<font color='red' size='6'><red>Hello PyQt5!\n单击按钮。</font>");
           //靠左显示
           layout->addWidget(head_icon_label);
           layout->addWidget(show_xiaoxi);
           layout->addStretch();
           w->setLayout(layout);
           ui->chatcontent->addItem(item);
           item->setSizeHint(QSize(ui->chatcontent->rect().width()-20,show_xiaoxi->rect().height()));
           ui->chatcontent->setItemWidget(item,w);
       }else{
           QString str="<!DOCTYPE HTML>";
                  str=str +"<html>"+
                          "<head>"+
                            "<style>"+
                                ".L_center_img{"+
                                 "max-height:40px;"+
                                 "max-width: 30px; "+
                                 "vertical-align:middle;"+
                             "}"+
                            "</style>"+
                          "</head>"+
                          "<body>"+
                            "<img class=\"L_center_img\" name=\"pic\" src=\"G:/QT workspace/test1/image/head_icon.jpg\" />"+
                          "</body>"+
                         "</html>";
           show_xiaoxi->setHtml(str);
           //靠右显示
           layout->addStretch();
           layout->addWidget(show_xiaoxi);
           layout->addWidget(head_icon_label);
           w->setLayout(layout);
           ui->chatcontent->addItem(item);
           item->setSizeHint(QSize(ui->chatcontent->rect().width()-20,show_xiaoxi->rect().height()));
           ui->chatcontent->setItemWidget(item,w);
       }
   }*/
}
void chatwindow::textAreaChanged()
{
    QTextDocument *document = qobject_cast<QTextDocument*>(sender());
    document->adjustSize();
    if(document){
        QTextEdit *editor = qobject_cast<QTextEdit*>(document->parent()->parent());
        if(editor){
            int newheight = document->size().rheight()+10;
            int newwidth = document->size().rwidth()+10;
            if(newheight!=editor->height()){
                editor->setFixedHeight(newheight);
            }
            if(newwidth!=editor->width()){
                editor->setFixedWidth(newwidth);
            }
        }
    }
}

void chatwindow::show_msg(QString str,int type)
{
    QListWidgetItem * item = new QListWidgetItem(ui->chatcontent,0);
    QWidget *w = new QWidget(ui->chatcontent);
    QHBoxLayout * layout = new QHBoxLayout(w);
    QString file = "G:/QT workspace/test1/image/head_icon.jpg";
    QPixmap pixmapa(file);
    QPixmap pixmap(40,40);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    QPainterPath path;
    path.addEllipse(1,1,40,40);
    painter.setClipPath(path);
    painter.drawPixmap(1,1,40,40,pixmapa);
    QLabel *head_icon_label = new QLabel(w);
    head_icon_label->setPixmap(pixmap);
    QTextEdit * show_xiaoxi = new QTextEdit(w);
    //设置文本只读，居中显示
    show_xiaoxi->setReadOnly(true);
    show_xiaoxi->setAlignment(Qt::AlignVCenter);
    //设置QTextEdit根据内容调整大小
    connect(show_xiaoxi->document(),SIGNAL(contentsChanged()),this,SLOT(textAreaChanged()));
    show_xiaoxi->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    show_xiaoxi->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    show_xiaoxi->setStyleSheet("QTextEdit { background: blue }");
    show_xiaoxi->setHtml(str);
    if(type==0)//发送消息
    {
        //靠右显示
        layout->addStretch();
        layout->addWidget(show_xiaoxi);
        layout->addWidget(head_icon_label);
        w->setLayout(layout);
    }else{//接收到的消息
        //靠左显示
        layout->addWidget(head_icon_label);
        layout->addWidget(show_xiaoxi);
        layout->addStretch();
        w->setLayout(layout);
    }
    ui->chatcontent->addItem(item);
    if(show_xiaoxi->rect().height()<70){
        item->setSizeHint(QSize(ui->chatcontent->rect().width()-20,70));
    }else{
        item->setSizeHint(QSize(ui->chatcontent->rect().width()-20,show_xiaoxi->rect().height()));
    }
    ui->chatcontent->setItemWidget(item,w);
}

QString chatwindow::produce_Html(QString str)//0是字符消息，1是文件消息
{
    QString head="<!DOCTYPE HTML>";
    head=head +"<html>"+
            "<head>"+
              "<style>"+
                  ".L_center_img{"+
                   "max-height:40px;"+
                   "max-width: 30px; "+
                   "vertical-align:middle;"+
               "}"+
              "</style>"+
            "</head>"+
            "<body>";
    QString tail = "</body></html>";
    QString return_str;
    return_str = head+str+tail;
    return return_str;
}
void chatwindow::on_send_clicked()
{
    QString str = ui->contentedit->toPlainText();
    if(str==""){
        QMessageBox::information(NULL,"tip","消息不能为空",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }else{
        QString msg = produce_Html(str);
        if(is_file){
            transt_file(str);
        }else{
           transt_str(msg);
        }
        show_msg(msg,0);
    }
    ui->contentedit->setText("");
}

void chatwindow::transt_file(QString file_name)
{
    file_socket = new QTcpSocket;
    file_socket->connectToHost(QHostAddress::LocalHost,6666);
    if(!file_socket->ConnectedState){
        QMessageBox::information(NULL,"tip","文件服务器连接失败",QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    }
    connect(file_socket,SIGNAL(bytesWritten(qint64)),this,SLOT(transt_file_body(qint64)));
    totalBytes = 0;
    byteWritten = 0;
    bytesToWrite = 0;
    loadSize = 4*1024;
    localfile = new QFile(file_name);
    if(!localfile->open(QFile::ReadOnly)){
        QMessageBox::critical(this,"错误","文件打开失败,传输终止","确定");
        return;
    }
    totalBytes = localfile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFileName = file_name.right(file_name.size()-file_name.lastIndexOf("/")-1);
    //发送两个0，加filename ；
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    //计算文件大小和文件名总大小
    totalBytes += outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
    bytesToWrite = totalBytes-file_socket->write(outBlock);
    outBlock.resize(0);
}

void chatwindow::transt_file_body(qint64 numBytes)
{
    byteWritten += (int)numBytes;
    if(bytesToWrite>0){
        //每次发送loadsize大小的数据，这里设置为4kb，如果剩余的数据不足4kb就发剩余的数据；
        outBlock = localfile->read(qMin(bytesToWrite,loadSize));
        bytesToWrite -= (int)file_socket->write(outBlock);
        outBlock.resize(0);
    }
    if(byteWritten == totalBytes){
        localfile->close();
        disconnect(file_socket,SIGNAL(bytesWritten(qint64)),this,SLOT(transt_file_body(qint64)));
        qDebug()<<"传输完成，关闭client";
        file_socket->close();
    }
}

void chatwindow::transt_str(QString msg)
{
    QString msg2 = QString::number(p_sender)+"#"+QString::number(receiver)+"#"+msg;
    QByteArray msg_array = msg2.toUtf8();
    tcp_client->write(msg_array);
}
void chatwindow::on_close_clicked()
{
    this->close();
}

void chatwindow::on_pushButton_clicked()
{
    open_file();
}
void chatwindow::open_file()
{
    QString file_name = QFileDialog::getOpenFileName(this);
    if(!file_name.isEmpty()){
        is_file = true;
        ui->contentedit->setText(file_name);
    }
}
