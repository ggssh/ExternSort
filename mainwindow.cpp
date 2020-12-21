﻿#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include <string>
#include <sstream>
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
using namespace std;

#define totallength 100  //生成数据量的大小
#define ways 3          //归并的路数
#define MINKEY -1       //败者树中的最小值

vector<vector<int>> gbc;//声明全局变量
int m_count=0;          //获得的归并段数量
int *LoserTree,*External;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(TR("计科1904-原毅哲-最佳归并树"));
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
    gbc.clear();
    ui->textBrowser->clear();
    init_data(totallength);
    select_replace(5);
    for(int i=0;i<gbc.size();i++){
        QString s="";
        for(int j=0;j<gbc[i].size();j++){
            s+=QString::number(gbc[i][j]);
            s+=" ";
        }
        ui->textBrowser->append(s);
    }
    //gbc.clear();
    best_merge_tree(4);
    write_data();
}
//Still have something wrong
void MainWindow::select_replace(int kk){
    FILE* fin = fopen("unsort_file.txt","rt");
    typedef struct buf
    {
        //缓冲区的数据
        int num;
        int flag=0;
    }buf;
    struct _cmp
    {
        //自定义优先队列比较函数,flag=1表示打过标记,flag相同的话小的排在前面,未被标记的排在队列的前面
        bool operator()(const buf& a, const buf& b) {
            if (a.flag == b.flag) return a.num > b.num;
            else return a.flag > b.flag;
        }
    };
    ui->progressBar->setValue(0);
    int count=0;//设置进度条的数值
    ui->progressBar->setRange(0,totallength);
    priority_queue<buf,vector<buf>,_cmp> temp;//利用优先队列作为选择置换排序缓冲区
    int aa;
    int n=0;
    buf a1,a2,a3;
    int t1=1;
    while(fscanf(fin,"%d",&aa)!=EOF){
        count++;
        ui->progressBar->setValue(count);
        if(t1){
            vector<int> ss;
            gbc.push_back(ss);
            t1=0;
        }
        a1.num=aa;
        temp.push(a1);
        if(temp.size()==kk){
            //当缓冲区长度满后输出其中最小的未被标记的元素的数值
            if(gbc[n].size()==0){
                gbc[n].push_back(temp.top().num);
                temp.pop();
            }
            else{
                while(temp.top().num<gbc[n][gbc[n].size()-1]){
                    a2=temp.top();
                    a2.flag=1;
                    temp.pop();
                    temp.push(a2);
                }
                gbc[n].push_back(temp.top().num);
                temp.pop();
            }
        }
        if(temp.top().flag==1){
            n++;
            while(temp.top().flag==1){
                a3=temp.top();
                a3.flag=0;
                temp.pop();
                temp.push(a3);
                if(temp.top().flag==0){
                    t1=1;
                    break;
                }
            }
        }
    }
    if(temp.size()==kk-1){
        /*
         * 此处有bug如果不再向vector中push_back一个元素的话可能会出现数组越界的情况
         * 但是添加一个元素的话可能会出现vector最后一个元素长度为0的情况
         */
        vector<int> cc;
        gbc.push_back(cc);
        while(!temp.empty()){
            gbc[n].push_back(temp.top().num);
            temp.pop();
        }
    }
    fclose(fin);
}

//no problem
void MainWindow::init_data(int n){
    //向文件中写入需要排序的数据
    //在主函数中定义宏来确定需要排序的数据量的大小
    srand(time(NULL));
    FILE* f = fopen("unsort_file.txt", "wt");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d ", rand());
    }
    fclose(f);
}

//no problem
void MainWindow::write_data(){
    //将经选择置换排序之后获得的数据依次写入每个文件中
     int n=0;
     for(int i=0;i<gbc.size();i++){
         if(gbc[i].size()) n++;
     }
     for(int i=0;i<n;i++){
         string s="temp";
         string ss;
         stringstream s1;
         s1<<i;
         s1>>ss;
         s+=ss;
         s+=".txt";
         const char* s2=s.c_str();
         FILE* fout=fopen(s2,"w+");
         for(int j=0;j<gbc[i].size();j++){
             fprintf(fout,"%d ",gbc[i][j]);
         }
         fclose(fout);
     }
}

void MainWindow::loser_merge(int temp1){
    cout<<temp1<<endl;

}

//TO DO
void MainWindow::best_merge_tree(int k){
    typedef struct node{
        int size;
        int locate;
    }node;
    struct _cmp
    {
        //自定义优先队列比较函数,flag=1表示打过标记,flag相同的话小的排在前面,未被标记的排在队列的前面
        bool operator()(const node& a, const node& b){
            return a.size>b.size;
        }
    };
    priority_queue<node,vector<node>,_cmp> a,b;
    int count=0;
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()) count++;
    }
    m_count=count;
    if((count-1)%(k-1)){
        for(int i=0;i<((k-1)-((count-1)%(k-1)));i++){
            node s;
            s.size=0;
            s.locate=-1;
            a.push(s);b.push(s);
        }
        cout<<"添加"<<(k-1)-((count-1)%(k-1))<<"个虚段"<<endl;
    }
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()){
            node s;
            s.size=gbc[i].size();
            s.locate=i;
            a.push(s);b.push(s);
        }
    }
    //cout<<b.size()<<endl;
    //b=a;
    while(!b.empty()){
        cout<<b.top().size<<" ";
        b.pop();
    }
    cout<<endl<<"以上为当前所有的归并串的长度 输出结束"<<endl;
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
    int guibing_count = 1;
    while(a.size()!=1){
        node s;
        s.size=0;
        /*
        for(int i=0;i<k;i++){
           if(a.empty()) break;
           s.size+=a.top().size;
           cout<<a.top().size<<" ";
           a.pop();
        }*/
        QString str=TR("第");
        str+=QString::number(guibing_count);
        str+=TR("次归并:");
        for(int i=0;i<k;i++){
            if(a.empty()) break;
            s.size+=a.top().size;
            str+=QString::number(a.top().size);
            str+=" ";
            a.pop();
        }
        QString str_temp = TR(" \n得到长度为")+QString::number(s.size)+TR("的顺串\n");
        str+=str_temp;
        ui->textBrowser_2->append(str);
        guibing_count++;
        //cout<<endl;
        a.push(s);
    }
    cout<<endl;
}

void MainWindow::Adjust(int s){
    //调整败者树
    int t=(s+ways)/2;
        int temp;
        while(t>0)
        {
            if(External[s] > External[LoserTree[t]])
            {
                temp = s;
                s = LoserTree[t];
                LoserTree[t]=temp;
            }
            t=t/2;
        }
        LoserTree[0]=s;
}

void MainWindow::CreateLoserTree(){
    //创建一个败者树
    External[ways]=MINKEY;
    int i;
    for(i=0;i<ways;i++)LoserTree[i]=ways;
    for(i=ways-1;i>=0;i--)Adjust(i);
}
