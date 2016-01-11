#include "login.h"
#include "ui_login.h"
#include "index.h"
#include <qmessagebox.h>
#include <cstringt.h>
#include <string.h>
using namespace std;
using namespace DataManagement;
using namespace UserManagement;

login::login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    ui->lineEdit->clear();ui->lineEdit_2->clear();
    aframe = new adminframe(this);
    aframe->hide();
    uframe = new userframe(this);
    uframe->hide();
    connect(this,SIGNAL(closing()),parent,SLOT(showagain()));
    connect(this,SIGNAL(sendAdmin(QString)),aframe,SLOT(receive(QString)));
    connect(this,SIGNAL(sendUser(QString)),uframe,SLOT(receive(QString)));
}

login::~login()
{
    delete ui;
}

void login::showagain(){
    this->show();
}

void login::on_pushButton_2_clicked()
{
    emit closing();
    reject();
}

void login::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("帐号或密码不能为空！"),QMessageBox::Yes);
    }
    else{
        QString account = ui->lineEdit->text();
        QString password = ui->lineEdit_2->text();
        User *tempuser = SelectUser(q_s(account));
        if(tempuser == nullptr){
            QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("没有该账户！"),QMessageBox::Yes);
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
        }
        else{
            if(!strcmp(tempuser->password, q_s(password).c_str())){
                if(!strcmp(tempuser->type,"admin")){
                    emit sendAdmin(ui->lineEdit->text());
                    ui->lineEdit->clear();
                    ui->lineEdit_2->clear();
                    aframe->show();
                    this->hide();
                }
                else{
                    emit sendUser(ui->lineEdit->text());
                    ui->lineEdit->clear();
                    ui->lineEdit_2->clear();
                    uframe->show();
                    this->hide();
                }
            }
            else{
                QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("用户名或密码错误！"),QMessageBox::Yes);
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
            }
        }
        delete tempuser;
    }

}
