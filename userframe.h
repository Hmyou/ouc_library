#ifndef USERFRAME_H
#define USERFRAME_H

#include <QDialog>
#include "usermanagement.h"
#include "bookmanagement.h"

namespace Ui {
class userframe;
}

class userframe : public QDialog
{
    Q_OBJECT

public:
    explicit userframe(QWidget *parent = 0);
    ~userframe();

private slots:
    void receive(QString);

    void on_exitButton_clicked();

    void on_changeButton_clicked();

    void on_lastButton_clicked();

    void on_nextButton_clicked();

    void on__15_clicked();

    void on__25_clicked();

    void on__35_clicked();

    void on__45_clicked();

signals:
    void exittoindex();

private:
    Ui::userframe *ui;
    QString accountReceive;
    void cleartxt();
    void printList(DataManagement::Record **list);
    vector<Record *> vt;
    vector<pair<Record *,int> > newVt;
    int nowCnt;
    Record* cmpTimeRecord;
    int prevNum, nextNum;
    bool overLimit;
};

#endif // USERFRAME_H
