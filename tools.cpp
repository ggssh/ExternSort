#include "tools.h"
#include "Windows.h"
void m_swap(int &a, int &b){
    a^=b;
    b^=a;
    a^=b;
}

char *temp_filename(int index){
    //创建临时文件
    char* tempfile = new char[100];
    sprintf(tempfile, "temp%d.txt", index);
    return tempfile;
}

void init_data(int n){
    //向文件中写入需要排序的数据
    srand(time(NULL));
    FILE* f = fopen("unsort_file.txt", "wt");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d ", rand());
    }
    fclose(f);
}
