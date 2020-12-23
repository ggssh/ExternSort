#include "huffman.h"
#include "ui_huffman.h"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
huffman::huffman(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huffman)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    //setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    this->setWindowTitle(TR("哈夫曼树"));
    this->setWindowIcon(QIcon(":/image/image/bg2.jpg"));
    static QMovie movie(":/image/image/huffman.png");
    movie.start();
    movie.resized(ui->label->size());
    ui->label->setMovie(&movie);
}

huffman::~huffman()
{
    delete ui;
}
