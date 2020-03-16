#ifndef WIDGET_H
#define WIDGET_H
#include <QModelIndex>
#include <QStringListModel>
#include <QStandardItemModel>
#include <QWidget>
#include "chatwindow.h"
#include "ui_widget.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    QStringListModel *Model;
    QStandardItemModel *ItemModel;
    chatwindow * chatWindow;
    int sender=205732204;
    int receiver=205731204;
    ~Widget();

private slots:

    void on_xiaoxi_clicked();

    void on_pushButton_clicked();

    void on_main_xiaoxi_itemDoubleClicked(QListWidgetItem *item);

public:
    Ui::Widget *ui;

};

#endif // WIDGET_H
