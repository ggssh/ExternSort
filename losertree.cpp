#include "losertree.h"
#include "ui_losertree.h"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
losertree::losertree(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::losertree)
{
    ui->setupUi(this);
    this->setWindowTitle(TR("败者树"));
    static QMovie movie(":/new/prefix1/gif/22.gif");
    movie.start();
    movie.resized(ui->label->size());
    ui->label->setMovie(&movie);
}

losertree::~losertree()
{
    delete ui;
}
