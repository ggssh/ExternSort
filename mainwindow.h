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

    void SelectReplace(int k);
    void WriteData();
    void LoserMerge(int k);
    void BestMergeTree(int kk);
    void Adjust(int s);
    void CreateLoserTree();
    void Initial();
    void DisplayMergeSegment();
};
#endif // MAINWINDOW_H
