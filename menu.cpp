#include "menu.h"
#include "ui_menu.h"

menu::menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::menu)
{
    ui->setupUi(this);
}

menu::~menu()
{
    delete ui;
}

 void getKernel(Ikernel *ikernel)
{
 }

void menu::on_pushButton_clicked()
{
    this->hide();
}


void menu::on_pushButton_2_clicked()
{
    
}

