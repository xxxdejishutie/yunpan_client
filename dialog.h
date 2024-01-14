#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include"ikerkel.h"


namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();


private slots:
    void on_button_queding_clicked();//注册按钮

    void on_pushButton_2_clicked();//登录按钮

private:
    Ui::Dialog *ui;

    Ikernel *m_ikernel;

public:
    void getkernel(Ikernel * kernel);//获得处理类


};

#endif // DIALOG_H
