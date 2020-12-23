#include "selectrp.h"
#include "ui_selectrp.h"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
selectrp::selectrp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::selectrp)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    //setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    this->setWindowTitle(TR("置换选择排序"));
    this->setWindowIcon(QIcon(":/image/image/bg2.jpg"));
    static QMovie movie(":/algorithm/gif/selectreplace.gif");
    movie.start();
    movie.resized(ui->label->size());
    ui->label->setMovie(&movie);
}

selectrp::~selectrp()
{
    delete ui;
}
