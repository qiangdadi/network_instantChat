#ifndef MESAPIC_CLASS_H
#define MESAPIC_CLASS_H

#endif // MESAPIC_CLASS_H
class QMesAPic:public QWidget
{
    Q_OBJECT
public:
    explicit QMesAPic(QWidget *parent=nullptr);
    void setMesAPic(QString text,);
protected:
    void paintEvent(QPaintEvent *event);
}
