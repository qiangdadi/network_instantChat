#include "receive_thread.h"
#include <QTcpSocket>
Receive_Thread::Receive_Thread(QTcpSocket *client_socket)
{
   tcp_clientsocket = client_socket;
}

void Receive_Thread::readServer()
{
     QByteArray msg_byte = tcp_clientsocket->readAll();
     msg = msg_byte;
     emit signal_msgCome(msg,1);
}

void Receive_Thread::ready_read()
{
    connect(tcp_clientsocket,SIGNAL(readyRead()),this,SLOT(readServer()));
}
