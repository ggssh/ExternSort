#include "huffman.h"
#include "ui_huffman.h"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
huffman::huffman(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::huffman)
{
    ui->setupUi(this);
    this->setWindowTitle(TR("哈夫曼树"));
}

huffman::~huffman()
{
    delete ui;
}
