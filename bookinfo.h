#ifndef BOOKINFO_H
#define BOOKINFO_H

#include <QDialog>
#include "data.h"
#include "index.h"
#include "bookmanagement.h"

namespace Ui {
class bookinfo;
}

class bookinfo : public QDialog
{
    Q_OBJECT

public:
    explicit bookinfo(QWidget *parent = 0);
    ~bookinfo();

private slots:
    void receive(QString bISBN);

private:
    Ui::bookinfo *ui;
    void tabelshow();
    void showRecord(Book *tbook, int i);
    void tabelshow2();
    void showRecord2(Book *tbook);
};

#endif // BOOKINFO_H
