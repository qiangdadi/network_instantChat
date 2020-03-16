#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QString>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QtNetwork>
#include "global.h"
#include <QElapsedTimer>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    //setWindowIcon(QIcon(":/new/icon/image/icon"));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_xiaoxi_clicked()
{
    for(int i=0;i<10;i++)
    {
        QListWidgetItem * item = new QListWidgetItem(ui->main_xiaoxi,0);
        //item->setFlags(Qt::ItemFlag());
        item->setFlags(Qt::ItemIsEnabled);
        QWidget *w = new QWidget(ui->main_xiaoxi);
        QHBoxLayout * hlayout = new QHBoxLayout(w);
        QVBoxLayout * vlayout = new QVBoxLayout(w);
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
        QLineEdit * main_name_edit = new QLineEdit(w);
        //设置失去焦点不可点击
        main_name_edit->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        main_name_edit->setObjectName("main_name_edit");
        //设置无边框
        main_name_edit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
        //设置字体
        QSize name_size(20,410);
        main_name_edit->resize(name_size);
        main_name_edit->setFont(QFont("Timers",15,QFont::Bold));
        //设置文本只读，居中显示
        main_name_edit->setReadOnly(true);
        main_name_edit->setAlignment(Qt::AlignVCenter);
        main_name_edit->setText("小强");
        QLineEdit * main_xiaoxi_edit = new QLineEdit(w);
        //设置不可点击
        main_xiaoxi_edit->setAttribute(Qt::WA_TransparentForMouseEvents,true);
        //设置无边框
        main_xiaoxi_edit->setStyleSheet("background:transparent;border-width:0;border-style:outset");
        main_xiaoxi_edit->setReadOnly(true);
        main_xiaoxi_edit->setAlignment(Qt::AlignVCenter);
        main_xiaoxi_edit->setText("这是一条消息");
        vlayout->addStretch();
        vlayout->addWidget(main_name_edit);
        vlayout->addWidget(main_xiaoxi_edit);
        vlayout->addStretch();
        hlayout->addWidget(head_icon_label);
        hlayout->addLayout(vlayout);
        w->setLayout(hlayout);
        ui->main_xiaoxi->addItem(item);
        item->setSizeHint(QSize(40,ui->xiaoxi->rect().width()));
        ui->main_xiaoxi->setItemWidget(item,w);
    }
}

void Widget::on_pushButton_clicked()
{

}

void Widget::on_main_xiaoxi_itemDoubleClicked(QListWidgetItem *item)
{
    //连接消息服务器
    tcp_client->connectToHost(QHostAddress::LocalHost,6665);
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<100);
    QString empty_msg = user_number + "#"+"0"+"#"+" ";//发送空消息;
    qDebug()<<user_number<<endl;
    QByteArray numberArray = empty_msg.toUtf8();
    tcp_client->write(numberArray);
    QString chatperson;
    QWidget *widget = ui->main_xiaoxi->itemWidget(item);
    QLineEdit *lineEdit = widget->findChild<QLineEdit *>("main_name_edit");
    chatperson = lineEdit->text().trimmed();
    chatWindow = new chatwindow(chatperson,sender,receiver);
    chatWindow->show();
}
