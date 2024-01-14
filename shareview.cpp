#include "shareview.h"
#include "ui_shareview.h"

shareview::shareview(QWidget *parent) :
    QGroupBox(parent),
    ui(new Ui::shareview)
{
    ui->setupUi(this);
}

shareview::~shareview()
{
    delete ui;
}
