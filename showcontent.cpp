#include "showcontent.h"
#include "ui_showcontent.h"

#define Filename "sort_file.txt"
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
ShowContent::ShowContent(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowContent)
{
    ui->setupUi(this);
    this->setWindowTitle(TR("以下为排序的结果"));
    this->setWindowIcon(QIcon(":/image/image/bg2.jpg"));
    this->move(1315,330);
    OpenText(Filename);
}

ShowContent::~ShowContent()
{
    delete ui;
}
bool ShowContent::OpenText(const QString &FileName){
    QFile aFile(FileName);
    if(!aFile.exists()) return false;
    if(!aFile.open(QIODevice::ReadOnly | QIODevice::Text)) return false;
    ui->textBrowser->setText(aFile.readAll());
    aFile.close();
    return true;
}
