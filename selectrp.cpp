#include "selectrp.h"
#include "ui_selectrp.h"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
selectrp::selectrp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectrp)
{
    ui->setupUi(this);
    this->setWindowTitle(TR("置换选择排序"));
    this->setWindowIcon(QIcon(":/image/image/bg2.jpg"));
}

selectrp::~selectrp()
{
    delete ui;
}
