#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include "tools.h"
#include <string>
#include <sstream>
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
using namespace std;

#define MINKEY -1       //败者树中的最小值
#define BUFFER 5        //置换选择排序中的缓冲区的大小

typedef int* LoserTree;
typedef int* External;

vector<vector<int>> gbc;//声明全局变量
int m_count=0;          //获得的归并段数量
int totallength;        //生成的数据量的大小
int ways;               //归并的路数

LoserTree ls;//败者树，定义为指针，之后生成动态数组
External b;//定义为指针，在成员函数中可以把它当作数组使用


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小
    this->setWindowTitle(TR("计科1904-原毅哲-最佳归并树"));
    this->setWindowIcon(QIcon(":/image/image/bg2.jpg"));
}

MainWindow::~MainWindow()
{
    //析构函数
    delete ui;
}

void MainWindow::on_buttonperform_clicked()
{
    //选择算法演示
    pm = new perform_menu();
    pm->show();
}

void MainWindow::on_buttonsort_clicked()
{
    if(showcontent){
        showcontent->close();
        delete showcontent;
        showcontent = NULL;
    }
    Initial();
    init_data(totallength);
    SelectReplace(BUFFER);
    DisplayMergeSegment();
    BestMergeTree(ways);
    WriteData();
    LoserMerge(m_count);
    if(!showcontent){
        showcontent = new ShowContent;
        showcontent->show();
    }
}
void MainWindow::SelectReplace(int k){
    FILE* fin = fopen("unsort_file.txt","rt");
    //定义选择置换排序中缓冲区中每个元素的结构类型
    typedef struct buf
    {
        int num;
        int flag=0;
    }buf;

    struct my_cmp
    {
        /*自定义优先队列比较函数
         * flag=1表示打过标记
         * flag相同的话小的排在前面,未被标记的排在队列的前面
         */
        bool operator()(const buf& a, const buf& b) {
            if (a.flag == b.flag) return a.num > b.num;
            else return a.flag > b.flag;
        }
    };

    ui->progressBar->setValue(0);//设置progressBar的当前值
    int count=0;//对数据量进行计数
    ui->progressBar->setRange(0,totallength);//设置progressBar的范围
    priority_queue<buf,vector<buf>,my_cmp> temp;//利用优先队列作为选择置换排序缓冲区
    int get_num;//从文件中读取的一个数字
    int n=0;
    buf temp1,temp2,temp3;
    int mark=1;
    while(fscanf(fin,"%d",&get_num)!=EOF){
        count++;
        ui->progressBar->setValue(count);
        if(mark){
            vector<int> vec_temp;
            gbc.push_back(vec_temp);
            mark=0;
        }
        temp1.num=get_num;
        temp.push(temp1);
        if(temp.size()==k){
            //当缓冲区长度满后输出其中最小的未被标记的元素的数值
            if(gbc[n].size()==0){
                gbc[n].push_back(temp.top().num);
                temp.pop();
            }
            else{
                while(temp.top().num<gbc[n][gbc[n].size()-1]){
                    temp2=temp.top();
                    temp2.flag=1;
                    temp.pop();
                    temp.push(temp2);
                }
                gbc[n].push_back(temp.top().num);
                temp.pop();
            }
        }
        if(temp.top().flag==1){
            n++;
            while(temp.top().flag==1){
                temp3=temp.top();
                temp3.flag=0;
                temp.pop();
                temp.push(temp3);
                if(temp.top().flag==0){
                    mark=1;
                    break;
                }
            }
        }
    }
    if(temp.size()==k-1){
        /*
         * 此处有bug如果不再向vector中push_back一个元素的话可能会出现数组越界的情况
         * 但是添加一个元素的话可能会出现vector最后一个元素长度为0的情况
         */
        vector<int> vec_temp;//避免出现数组越界的情况
        gbc.push_back(vec_temp);
        while(!temp.empty()){
            gbc[n].push_back(temp.top().num);
            temp.pop();
        }
    }
    fclose(fin);//关闭已经打开的文件
}

/*
void MainWindow::init_data(int n){
    //向文件中写入需要排序的数据
    srand(time(NULL));
    FILE* f = fopen("unsort_file.txt", "wt");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d ", rand());
    }
    fclose(f);
}*/

void MainWindow::WriteData(){
    //将经选择置换排序之后获得的数据依次写入每个文件中
     int n=0;//记录归并段的数量
     for(int i=0;i<gbc.size();i++){
         if(gbc[i].size()) n++;
     }
     for(int i=0;i<n;i++){
         string outstr="temp";
         string tempstr;
         stringstream sstr;
         sstr<<i;
         sstr>>tempstr;
         outstr+=tempstr;
         outstr+=".txt";
         const char* sout=outstr.c_str();
         FILE* fout=fopen(sout,"w+");
         for(int j=0;j<gbc[i].size();j++){
             fprintf(fout,"%d ",gbc[i][j]);
         }
         fprintf(fout,"%d",INT_MAX);
         fclose(fout);
     }
     gbc.clear();//清理内存
}

void MainWindow::LoserMerge(int k){
    ls = new int[k];
    b = new int[k+1];
    FILE *fout = fopen("sort_file.txt","wt");
    FILE** farray = new FILE*[k];
    for(int i=0;i<k;i++){
        //打开所有k路输入文件
        char* fileName = temp_filename(i);
        farray[i] = fopen(fileName,"rt");
        free(fileName);
    }
    for(int i=0;i<k;i++){
        //初始读取第一个顺串向b[0]中写入数据，依次类推
        if(fscanf(farray[i],"%d",&b[i])==EOF){
            //读取每个文件的第一个数到data数组
            return;
        }
    }
    CreateLoserTree();//创建一个败者树
    int q;
    while(b[ls[0]]!=INT_MAX){
        q=ls[0];
        fprintf(fout,"%d ",b[q]);
        fscanf(farray[q],"%d",&b[q]);
        Adjust(q);
    }
    for(int i=0;i<k;i++){
        fclose(farray[i]);//依次关闭每个FILE
    }
    delete[] farray;
    delete[] ls;
    delete[] b;
    fclose(fout);
}

void MainWindow::BestMergeTree(int k){
    typedef struct node{
        int size;
    }node;

    /*
     * 自定义优先队列比较函数
     * flag=1表示打过标记,flag相同的话小的排在前面
     * 未被标记的排在队列的前面
     */
    struct _cmp
    {
        bool operator()(const node& a, const node& b){
            return a.size>b.size;
        }
    };
    priority_queue<node,vector<node>,_cmp> priQueueA,priQueueB;//创建两个优先队列来实现哈夫曼树
    int count=0;
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()) count++;
    }
    m_count=count;//对归并串的数量进行赋值
    if((count-1)%(k-1)){
        //判断是否需要添加虚段
        for(int i=0;i<((k-1)-((count-1)%(k-1)));i++){
            node s;
            s.size=0;
            //s.locate=-1;
            priQueueA.push(s);
            priQueueB.push(s);
        }
        QString str = TR("添加") + QString::number((k-1)-((count-1)%(k-1))) + TR("个虚段");
        ui->textBrowser_2->append(str);
    }
    else ui->textBrowser_2->append(TR("不需要添加虚段"));
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()){
            node s;
            s.size=gbc[i].size();
            //s.locate=i;
            priQueueA.push(s);
            priQueueB.push(s);
        }
    }
    /*
    while(!b.empty()){
        cout<<b.top().size<<" ";
        b.pop();
    }
    cout<<endl<<"以上为当前所有的归并串的长度 输出结束"<<endl;
    */
    /*
     *
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()){
            node s;
            s.size=gbc[i].size();
            s.locate=i;
            a.push(s);
        }
    }
    */
    int merge_count = 1;
    while(priQueueA.size()!=1){
        node res;
        res.size=0;
        /*
        for(int i=0;i<k;i++){
           if(a.empty()) break;
           s.size+=a.top().size;
           cout<<a.top().size<<" ";
           a.pop();
        }*/
        QString str=TR("第");
        str+=QString::number(merge_count);
        str+=TR("次归并:");
        for(int i=0;i<k;i++){
            if(priQueueA.empty()) break;
            res.size+=priQueueA.top().size;
            str+=QString::number(priQueueA.top().size);
            str+=" ";
            priQueueA.pop();
        }
        QString str_temp = TR("\n得到长度为")+QString::number(res.size)+TR("的顺串\n");
        str+=str_temp;
        ui->textBrowser_2->append(str);
        merge_count++;
        priQueueA.push(res);
    }
    ui->textBrowser_2->append(TR("归并成功"));
}

void MainWindow::Adjust(int s){
    //调整败者树
    //沿从叶子结点b[s]到根结点ls[0]的路径调整败者树
    int t=(s+m_count)/2;//l[t]是b[s]的双亲节点
        //int temp;
        while(t>0)
        {
            if(b[s] > b[ls[t]])
            {
                /*
                temp = s;
                s = ls[t];
                ls[t]=temp;
                */
                m_swap(s,ls[t]);
            }
            t=t/2;
        }
        ls[0]=s;//ls[0]存放调整后的最大值的位置
}
/*
char * MainWindow::temp_filename(int index){
    //创建临时文件
    char* tempfile = new char[100];
    sprintf(tempfile, "temp%d.txt", index);
    return tempfile;
}*/

void MainWindow::CreateLoserTree(){
    //创建一个败者树
    b[m_count]=MINKEY;
    for(int i=0;i<m_count;i++) ls[i]=m_count;
    for(int i=m_count-1;i>=0;i--)Adjust(i);
}

void MainWindow::Initial(){
    //对全局变量以及控件进行初始化
    gbc.clear();
    totallength=ui->spinBox1->value();
    ways=ui->spinBox2->value();
    ui->textBrowser->clear();
    ui->textBrowser_2->clear();
}

void MainWindow::DisplayMergeSegment(){
    for(int i=0;i<gbc.size();i++){
        QString s=TR("第");
        s+=QString::number(i+1);
        s+=TR("个归并段长度为:");
        if(gbc[i].size()){
            s+=QString::number(gbc[i].size());
            ui->textBrowser->append(s);
        }
    }
}
