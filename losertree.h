#ifndef LOSERTREE_H
#define LOSERTREE_H

#include <QWidget>
#include <QMovie>
#include <QIcon>

namespace Ui {
class losertree;
}

class losertree : public QWidget
{
    Q_OBJECT

public:
    explicit losertree(QWidget *parent = nullptr);
    ~losertree();

private:
    Ui::losertree *ui;
};

#endif // LOSERTREE_H
