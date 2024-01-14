#include "dialog.h"
#include<string>
#include"packdef.h"
#include"ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::getkernel(Ikernel * kernel)
{
   m_ikernel = kernel;
}

void Dialog::on_button_queding_clicked()//注册
{
    QString name,tel,password;
    tel = ui->lineEdit->text();//获得电话号
    name = ui->lineEdit_2->text();//获得用户名
    password = ui->lineEdit_3->text();//获得密码


    STRU_REGISTER_RQ rq;

    rq.m_nType = _DEF_PROTOCOL_REGISTER_RQ;

   rq.m_ltel = tel.toLongLong();
   strcpy(rq.m_szName,name.toStdString().c_str());
   strcpy(rq.m_szPassword,password.toStdString().c_str());

   m_ikernel->SendData((char*)&rq,sizeof(rq));//发送注册请求包





}

void Dialog::on_pushButton_2_clicked()//登录
{
    QString name,tel,password;
    tel = ui->lineEdit->text();//获得电话号
    name = ui->lineEdit_2->text();//获得用户名
    password = ui->lineEdit_3->text();//获得密码

    STRU_LOGIN_RQ rq;
    rq.m_nType = _DEF_PROTOCOL_LOGIN_RQ;
    strcpy(rq.m_szName,name.toStdString().c_str());
    strcpy(rq.m_szPassword,password.toStdString().c_str());

    m_ikernel->SendData((char*)&rq,sizeof(rq));//发送登录请求包

}
