#include "perform_menu.h"
#include "ui_perform_menu.h"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
perform_menu::perform_menu(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::perform_menu)
{
    ui->setupUi(this);
    this->setWindowTitle(TR("算法演示"));
    this->setWindowIcon(QIcon(":/image/image/bg2.jpg"));
}

perform_menu::~perform_menu()
{
    delete ui;
}

void perform_menu::on_Buttonselectrp_clicked()
{
    strp = new selectrp();
    strp->show();
}

void perform_menu::on_ButtonLoser_clicked()
{
    lt = new losertree();
    lt->show();
}

void perform_menu::on_Buttonhuffman_clicked()
{
    huff = new huffman();
    huff->show();
}
