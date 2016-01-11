#ifndef ADDACCOUNT_H
#define ADDACCOUNT_H

#include <QDialog>
#include "usermanagement.h"

namespace Ui {
class addaccount;
}

class addaccount : public QDialog
{
    Q_OBJECT

public:
    explicit addaccount(QWidget *parent = 0);
    ~addaccount();

private slots:
    void on_pushButton_2_clicked();

    void on_addButton_clicked();

    void on_lineEdit_textEdited();

    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::addaccount *ui;
    int getRadio();
};

#endif // ADDACCOUNT_H
