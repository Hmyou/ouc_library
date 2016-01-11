#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "index.h"
#include "data.h"
#include "usermanagement.h"
#include "adminframe.h"
#include "userframe.h"


namespace Ui {
class login;
}

class login : public QDialog
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = 0);
    ~login();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void showagain();


signals:
    void closing();
    void sendAdmin(QString);
    void sendUser(QString);

private:
    Ui::login *ui;
    adminframe *aframe;
    userframe *uframe;
};

#endif // LOGIN_H
