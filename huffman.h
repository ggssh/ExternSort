#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <QWidget>
#include <QLabel>
#include <QMovie>

namespace Ui {
class huffman;
}

class huffman : public QWidget
{
    Q_OBJECT

public:
    explicit huffman(QWidget *parent = nullptr);
    ~huffman();

private:
    Ui::huffman *ui;
};

#endif // HUFFMAN_H
