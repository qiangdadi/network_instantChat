#include "filethread.h".h"
#include <QThread>
#include <QTcpSocket>
#include <QObject>
#include <QByteArray>
#include <QString>
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QMessageBox>
FileThread::FileThread(QTcpSocket *clientsocket)
{
    tcp_clientsocket = clientsocket;
}

void FileThread::run()
{
    connect(tcp_clientsocket,SIGNAL(readyRead()),this,SLOT(readClient()));
}

void FileThread::readClient()
{
    QDataStream in(tcp_clientsocket);
    in.setVersion(QDataStream::Qt_4_6);
    if(bytesReceived <= sizeof(qint64)*2)
    {
        if((tcp_clientsocket->bytesAvailable()>=sizeof(qint64)*2)&&(fileNameSize==0))//接收文件大小信息
        {
            in>>totalBytes>>fileNameSize;
            bytesReceived += sizeof(qint64)*2;
        }
        if(tcp_clientsocket->bytesAvailable() >= fileNameSize)//如果是文件名字部分
        {
            in>>fileName;
            qDebug()<<fileName<<"\n";
            bytesReceived += fileNameSize;
            localFile = new QFile(fileName);
            if(!localFile->open(QFile::WriteOnly))
            {
                //QMessageBox::critical(this,"错误","文件打开失败，文件传输终止","确定");
                return;
            }
        }else{
            return;
        }
    }
    if(bytesReceived<totalBytes)//接收文件body部分
    {
        bytesReceived += tcp_clientsocket->bytesAvailable();
        inBlock = tcp_clientsocket->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    if(bytesReceived == totalBytes)
    {
        localFile->close();
        tcp_clientsocket->close();
        msg_str = "客户端" + tcp_clientsocket->objectName() + fileName+"文件传输完成";
        emit signal_updateUI();
    }
}
