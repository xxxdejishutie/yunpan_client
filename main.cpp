#include "widget.h"

#include <QApplication>
#include<QDebug>
#include<iostream>


int main(int argc, char *argv[])
{
     //若英文系统，则用GBK
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    std::cout << "start " << endl;
    QApplication a(argc, argv);
    Widget w;
    w.hide();
    //w.show();


   // system("pause");
    return a.exec();
}
