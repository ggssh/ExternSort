#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "perform_menu.h"
#include "showcontent.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <queue>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_buttonperform_clicked();
    void on_buttonsort_clicked();

private:
    Ui::MainWindow *ui;
    perform_menu *pm;
    ShowContent *showcontent=NULL;

    void select_replace(int k);
    void init_data(int n);
    void write_data();
    void loser_merge(int k);
    void best_merge_tree(int kk);
    void Adjust(int s);
    void CreateLoserTree();
    char *temp_filename(int index);
    void Initial();
    void displayMergeSegment();
};
#endif // MAINWINDOW_H
