#ifndef PERFORM_MENU_H
#define PERFORM_MENU_H

#include <QWidget>
#include "selectrp.h"
#include "losertree.h"
#include "huffman.h"

namespace Ui {
class perform_menu;
}

class perform_menu : public QWidget
{
    Q_OBJECT

public:
    explicit perform_menu(QWidget *parent = nullptr);
    ~perform_menu();

private slots:
    void on_Buttonselectrp_clicked();

    void on_ButtonLoser_clicked();

    void on_Buttonhuffman_clicked();

private:
    Ui::perform_menu *ui;
    selectrp* strp;
    losertree* lt;
    huffman* huff;
};

#endif // PERFORM_MENU_H
