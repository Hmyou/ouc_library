#ifndef ADMINFRAME_H
#define ADMINFRAME_H

#include <QDialog>
#include "data.h"
#include "addaccount.h"
#include "addbook.h"
#include "usermanagement.h"
#include "bookmanagement.h"

namespace Ui {
class adminframe;
}

class adminframe : public QDialog
{
    Q_OBJECT

public:
    explicit adminframe(QWidget *parent = 0);
    ~adminframe();

private slots:
    void on_exitButton_clicked();

    void on_adduserButton_clicked();

    void showagain();

    void receive(QString);

    void on_changeButton_clicked();

    void on_searchButton_clicked();

    void on_changeButton_2_clicked();

    void on_deleteButton_clicked();

    void on_addbookButton_clicked();

    void on_pushButton_clicked();

    void on_borrowButton_clicked();

    void on_bottom3_clicked();

    void on_bottom4_clicked();

    void on_returnButton_clicked();

    void on_deleteBook_clicked();

    void on_pushButton_2_clicked();


signals:
    void exittoindex();

private:
    Ui::adminframe *ui;
    addaccount* addAccountFrame;
    addbook* addBookFrame;
    QString accountReceive;
    void hidetxtbtn();
    void showtxtbtn();
    void bottomHide();
    void bottomShow();
    void resultShow();
    void resultHide();
    int searchID;
    int flag;
};

#endif // ADMINFRAME_H
