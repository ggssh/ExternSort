#ifndef SELECTRP_H
#define SELECTRP_H

#include <QWidget>
#include <QIcon>

namespace Ui {
class selectrp;
}

class selectrp : public QWidget
{
    Q_OBJECT

public:
    explicit selectrp(QWidget *parent = nullptr);
    ~selectrp();

private:
    Ui::selectrp *ui;
};

#endif // SELECTRP_H
