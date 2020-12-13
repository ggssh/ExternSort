#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Windows.h"
#include <string>
#include <sstream>
#define TR(str)   (QString::fromLocal8Bit(str))  //解决中文乱码
#define totallength 20
#define k 2//选择二路归并
#define MINKEY -1
using namespace std;
vector<vector<int>> gbc;//声明全局变量
int _count=0;
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

/*
void MainWindow::on_pushButton_clicked()
{
    ui->progressBar->setValue(0);
    ui->textBrowser->clear();
    ui->progressBar->setRange(0,totallength);
    int num=0;
    for(int i=0;i<totallength;i++){
        num+=i;
        ui->progressBar->setValue(i*100/totallength);
        Sleep(10);
    }
    QString s=QString::number(num);
    ui->textBrowser->append(s);
    ui->progressBar->setValue(totallength);
}
void MainWindow::on_pushButton_2_clicked()
{
    //srand(time(NULL));
    FILE* f = fopen("unsort_file.txt", "wt");
    for (int i = 0; i < 10000; i++) {
        fprintf(f, "%d ", rand());
    }
    fclose(f);
    ui->progressBar->setValue(0);
    ui->progressBar->setRange(0,10000);

}
*/
void MainWindow::on_buttonperform_clicked()
{
    //选择算法演示
    pm = new perform_menu();
    pm->show();
}

void MainWindow::on_buttonsort_clicked()
{
    /*
     * 对文件进行外部排序
     * 1.void init_data(int n) n为生成的数据量
     * 2.void select_replace() 使用置换选择排序来将初始文件分为多个归并段,并在进度条中显示
     *  考虑使用sleep()来让程序输出减慢 将数据写入文件中
     * 3.void huffman() 根据每个文件的长度来构建一个最佳的哈夫曼树
     */
    gbc.clear();
    ui->textBrowser->clear();
    init_data(totallength);
    select_replace(5);
    for(int i=0;i<gbc.size();i++){
        QString s="";
        for(int j=0;j<gbc[i].size();j++){
            s+=QString::number(gbc[i][j]);
            s+=" ";
            //cout<<gbc[i][j]<<" ";
        }
        //cout<<endl;
        ui->textBrowser->append(s);
        Sleep(200);
    }
    best_merge_tree(5);
    write_data();
}

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
    int count=0;
    ui->progressBar->setRange(0,totallength);
    priority_queue<buf,vector<buf>,_cmp> temp;
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

void MainWindow::write_data(){
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

void MainWindow::loser_merge(int kk){

}

void MainWindow::best_merge_tree(int kk){
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
    priority_queue<node,vector<node>,_cmp> a;
    int count=0;
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()) count++;
    }
    _count=count;
    if((count-1)%(kk-1)){
        for(int i=0;i<((kk-1)-((count-1)%(kk-1)));i++){
            node s;
            s.size=0;
            s.locate=-1;
            a.push(s);
        }
        cout<<"添加"<<(kk-1)-((count-1)%(kk-1))<<"个虚段"<<endl;
    }
    while(!a.empty()){
        cout<<a.top().size<<" ";
        a.pop();
    }
    for(int i=0;i<gbc.size();i++){
        if(gbc[i].size()){
            node s;
            s.size=gbc[i].size();
            s.locate=i;
            a.push(s);
        }
        while(a.size()!=1){
            node s;
            s.size=0;
            for(int i=0;i<kk;i++){
                if (a.empty()) break;
               s.size+=a.top().size;
               cout<<a.top().size<<" ";
               a.pop();
            }
            cout<<endl;
            a.push(s);
        }
    }
}

void MainWindow::Adjust(int s){
    int t=(s+k)/2;
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
    External[k]=MINKEY;
    int i;
    for(i=0;i<k;i++)LoserTree[i]=k;
    for(i=k-1;i>=0;i--)Adjust(i);
}
