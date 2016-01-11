#ifndef ADDBOOK_H
#define ADDBOOK_H

#include <QDialog>
#include "bookmanagement.h"


namespace Ui {
class addbook;
}

class addbook : public QDialog
{
    Q_OBJECT

public:
    explicit addbook(QWidget *parent = 0);
    ~addbook();

private slots:
    void on_pushButton_2_clicked();


    void on_pushButton_clicked();

    void on_isbnEdit_textEdited();

    void on_priceEdit_textEdited();

    void on_numEdit_textEdited();

    void on_nameEdit_textEdited();

    void on_authorEdit_textEdited();

    void on_pubEdit_textEdited();

private:
    Ui::addbook *ui;
    void clearbookinfo();
};

#endif // ADDBOOK_H
