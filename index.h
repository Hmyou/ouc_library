#ifndef INDEX_H
#define INDEX_H

#include <QMainWindow>
#include <string>
#include "login.h"
#include "bookinfo.h"
#include "data.h"
#include "bookmanagement.h"


namespace Ui {
    class index;
}
class login;
class bookinfo;

class index : public QMainWindow
{
    Q_OBJECT

public:
    int getRadioKind();
    void hideTxt();
    void clearTxt();
    void printBookList(DataManagement::Book** list);
    explicit index(QWidget *parent = 0);
    ~index();

private slots:

    void showagain();

    void on_loginButton_clicked();

    void on_searchButton_clicked();

    void on_lastButton_clicked();

    void on_nextButton_clicked();

    void on__15_clicked();

    void on__25_clicked();

    void on__35_clicked();

    void on__45_clicked();

signals:
    void send(QString ISBN);

private:
    Ui::index *ui;
    vector<DataManagement::Book *> vt;
    vector< pair<DataManagement::Book *,int> > newVt;
    int nowCnt;
    int prevNum,nextNum;
    login *log;
    bookinfo *bookFrame;
};

#endif // INDEX_H
