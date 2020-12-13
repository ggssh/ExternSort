#ifndef EXTERN_SORT_H
#define EXTERN_SORT_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <algorithm>
using namespace std;

#define MIN -1
#define MAX 10000000//最大值，附加在归并文件结尾
typedef int* LoserTree;
typedef int* External;
typedef int* Workplace;

class ExternSort {
public:
    void _sort() {
        time_t start = time(NULL);
        //将文件内容分块在内存中排序，并分别写入临时文件
        k = memory_sort();

        //归并临时文件到输出文件
        ls = new int[k];
        b = new int[k + 1];
        K_Merge();

        delete[]ls;
        delete[]b;
        //display();
        time_t end = time(NULL);
        //printf_s("总共用时%f s\n", (end - start) * 1000.0 / CLOCKS_PER_SEC);
        cout << fixed << setprecision(2) << "总共用时" << (end - start)*1000.0 / (CLOCKS_PER_SEC*1.0) << "ms" << endl;
    }
    ExternSort(const char* input_file, const char* out_file, int count) {
        //input_file:输入文件名
        //out_file:输出文件名
        //count:每次在内存中排序的整数个数
        m_count = count;
        m_in_file = new char[strlen(input_file) + 1];
        strcpy(m_in_file, input_file);
        m_out_file = new char[strlen(out_file) + 1];
        strcpy(m_out_file, out_file);
    }
    virtual ~ExternSort() {
        delete[]m_in_file;
        delete[]m_out_file;
    }
private:
    int m_count;//数组长度
    char* m_in_file;//输入文件的路径
    char* m_out_file;//输出文件的路径
    int k;//归并数，此数必须要内排序之后才能得到，所以下面的ls和b都只能定义为指针
    LoserTree ls;//,败者树，定义成为指针，之后动态生成数组
    LoserTree rpls;//选择置换排序中为了从工作区中选出一个最小的值,每次向工作区中添加一个数字后进行选择败者树的调整
    int rpls_num;//选择置换排序中最小堆的元素个数，当其等于0时，说明一个归并串生成成功
    External b;//定义成为指针，在成员函数中可以把它当作数组使用
    Workplace wp;//置换选择排序的工作区

protected:
    int read_data(FILE* f, int a[], int n) {
        int i = 0;
        while (i < n && (fscanf_s(f, "%d", &a[i]) != EOF))i++;
        if (i) //printf_s("已读入:%d 个整数\n", i);
        //返回读入的整数的个数，有可能会小于n
        return i;
    }
    void write_data(FILE* f, int a[], int n) {
        for (int i = 0; i < n; i++) {
            fprintf_s(f, "%d ", a[i]);
        }
        fprintf_s(f, "%d ", MAX);//在最后写上一个最大值
    }
    char* temp_filename(int index) {
        char* tempfile = new char[100];
        sprintf(tempfile, "temp%d.txt", index);
        return tempfile;
    }
    static int cmp_int(const void* a, const void* b) {
        //自定义比较函数
            return *(int*)a - *(int*)b;
        }
    static bool cmp(const int& a, const int& b) {
        //自定义比较函数
        return a < b;
    }
    int memory_sort() {
        FILE* fin = fopen(m_in_file, "rt");
        int n = 0;
        int file_count = 0;
        int* array = new int[m_count];
        //每读入m_count个整数就在内存中做一次排序，并写入临时文件
        while ((n = read_data(fin, array, m_count)) > 0)
        {
            //qsort(array, n, sizeof(int), cmp_int);
            //经测试发现STL中的sort还是要比qsort快上不少的，只不过是因为debug模式会添加一些其他的东西导致时间变长
            sort(array, array + n, cmp);
            //cout << "内部排序结果为:" << endl;
           // for (int i = 0; i < n; i++) {
            //    cout << array[i] << " ";
            //}
           // cout << endl;
            char* fileName = temp_filename(file_count++);//生成临时文件
            FILE* tempFile = fopen(fileName, "w");//以可写的方式建立一个tempFile文件对象
            free(fileName);
            write_data(tempFile, array, n);//将排序后的数组写入至临时文件
            fclose(tempFile);//关闭文件流
        }
        delete[] array;//new完之后对应的时delete
        fclose(fin);//关闭已经打开的文件
        return file_count;//返回生成的归并段的数目
    }
    void _swap(int& a, int& b) {
        //交换函数
        a = a ^ b;
        b = b ^ a;
        a = a ^ b;
    }
    void Adjust(int s) {
        //沿从叶子节点b[s]到根结点ls[0]的路径调整败者树
        int t = (s + k) / 2;//ls[t]是b[s]的双亲节点
        while (t) {
            if (b[s] > b[ls[t]]) {
                //如果失败，则失败者位置s留下，s指向新的胜利者
                //int temp = s;
                //s = ls[t];
                //ls[t] = temp;
                _swap(s, ls[t]);
            }
            t = t / 2;
        }
        ls[0] = s;//ls[0]存放调整后的最大值的位置
    }
    void RpAdjust(int s) {
        //沿从叶子节点wp[s]到根结点rpls[0]的路径调整败者树
        int t = (s + k) / 2;//rpls[t]是wp[s]的双亲节点
        while (t) {
            //这个要求的是最小值
            if (b[s] > b[rpls[t]]) {
                //如果失败，则失败者位置s留下，s指向新的胜利者
                //int temp = s;
                //s = ls[t];
                //ls[t] = temp;
                _swap(s, rpls[t]);
            }
            t = t / 2;
        }
        rpls[0] = s;//ls[0]存放调整后的最小值的位置
    }
    void CreateLoserTree() {
        //额外的储存一个最小值
        b[k] = MIN;
        //先初始化为指向最小值，这样后面的调整才是正确的
        //这样才能够保证非叶子节点都是子树中的“二把手”
        for (int i = 0; i < k; i++) ls[i] = k;
        //依次从b[k-1],b[k-2],b[k-3]...出发调整败者树
        for (int i = k - 1; i >= 0; i--) Adjust(i);
    }
    void CreateRpLs() {
        FILE* fin = fopen(m_in_file, "r");
        rpls_num = 0;//初始小顶堆的大小为0
        for (int i = 0; i < 5; i++) {
            //置换选择排序中使用的缓冲区最大为5，每次与归并串最后一位进行比较后输入小顶堆堆顶元素
            if (fscanf(fin, "%d", &wp[i]) == EOF) break;
            rpls_num++;
        }
    }
    void K_Merge() {
        //利用败者树把k个输入归并段归并到输出段中
        //b中前k个变量存放k个输入段中当前记录的元素
        //归并临时文件
        FILE* fout = fopen(m_out_file, "wt");
        FILE** farray = new FILE * [k];
        for (int i = 0; i < k; i++) {
            //打开所有k路输入文件
            char* fileName = temp_filename(i);
            farray[i] = fopen(fileName, "rt");
            free(fileName);
        }
        for (int i = 0; i < k; i++) {
            //初始读取,第一个顺串的向b[0]中写入数据，依次类推
            if (fscanf_s(farray[i], "%d", &b[i]) == EOF) {
                //读每个文件的第一个数到data数组
                //printf_s("there is no %d file to merge!", k);
                //先注释掉吧，以免出现bug
                return;
            }
        }
        CreateLoserTree();
        int q;
        while (b[ls[0]] != MAX) {
            q = ls[0];
            fprintf_s(fout, "%d ", b[q]);
            fscanf_s(farray[q], "%d", &b[q]);
            Adjust(q);
        }
        //fprintf_s(fout, "%d ", b[ls[0]]);//其实没有必要在输出文件的最后写一个MAX的
        for (int i = 0; i < k; i++) {
            fclose(farray[i]);
        }
        delete[]farray;
        fclose(fout);
    }
    void display() {
        //将排序后的文件进行输出
        FILE* fin = fopen(m_out_file, "r");
        //int a[100];
        cout << "对该文件进行外部排序后的结果为:" << endl;
        int temp;
        int i = 0;
        while (fscanf(fin, "%d", &temp) != EOF) {
            i++;
            cout << temp << " ";
            if (i % 10 == 0 && i) cout << endl;
        }
    }
};

#endif // EXTERN_SORT_H
