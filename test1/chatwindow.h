#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QSize>
#include <QFile>
#include <QTcpSocket>
#include <QThread>
#include "receive_thread.h"

namespace Ui {
class chatwindow;
}

class chatwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit chatwindow(QString chatPerson,int p_sender,int receiver,QWidget *parent = 0);
    void qlistview_Adapter();
    ~chatwindow();
private slots:
    void textAreaChanged();
    void on_send_clicked();
    void on_close_clicked();
    void on_pushButton_clicked();
    void show_msg(QString str,int type);
    void transt_file_body(qint64);

private:
    int p_sender;
    int receiver;
    Ui::chatwindow *ui;
    bool is_file = false;
    QTcpSocket *file_socket;
    QFile *localfile;
    qint64 totalBytes;
    qint64 byteWritten;
    qint64 bytesToWrite;
    qint64 loadSize;
    QByteArray outBlock;
    Receive_Thread *receive_entity;
    QThread *receive_thread;
    void open_file();
    void transt_file(QString file_name);
    void transt_str(QString msg);
    QString produce_Html(QString str);

};

#endif // CHATWINDOW_H
