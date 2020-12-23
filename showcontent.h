#ifndef SHOWCONTENT_H
#define SHOWCONTENT_H

#include <QWidget>
#include <QFile>
#include <QIcon>

namespace Ui {
class ShowContent;
}

class ShowContent : public QWidget
{
    Q_OBJECT

public:
    explicit ShowContent(QWidget *parent = nullptr);
    ~ShowContent();

private:
    Ui::ShowContent *ui;
    bool openText(const QString &FileName);
};

#endif // SHOWCONTENT_H
