#include "addaccount.h"
#include "ui_addaccount.h"
#include "data.h"
#include <qmessagebox.h>

using namespace UserManagement;

addaccount::addaccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addaccount)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setFixedSize(this->width(), this->height());
}

addaccount::~addaccount()
{
    delete ui;
}

void addaccount::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    this->hide();
}

int addaccount::getRadio(){
    if(ui->radioButton->isChecked())
        return 1;
    else if(ui->radioButton_2->isChecked())
        return 2;
    else
        return -1;
}

void addaccount::on_addButton_clicked()
{
    if(ui->lineEdit->text().isEmpty()||ui->lineEdit_2->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请填入帐号密码!!"),QMessageBox::Ok);
    }
    else{
        if(ui->warn->text()!=""){
            QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("存在输入错误，请查看红色错误提示！！"),QMessageBox::Ok);
            return;
        }
        QString tmpAccount = ui->lineEdit->text();
        QString tmpPassword = ui->lineEdit_2->text();
        int tmpType = getRadio();
        string userT = "user";string adminT = "admin";
        if(tmpType == 1){
            int result = Addaccount(q_s(tmpAccount),q_s(tmpPassword),userT);
            if(result == -2){
                QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("已经存在该帐号!!"),QMessageBox::Ok);
                ui->lineEdit_2->clear();
            }
            else{
                QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("添加成功！"),QMessageBox::Ok);
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
            }
        }
        else if(tmpType == 2){
            int result_2 = Addaccount(q_s(tmpAccount),q_s(tmpPassword),adminT);
            if(result_2 == -2){
                QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("已经存在该帐号!!"),QMessageBox::Ok);
                ui->lineEdit_2->clear();
            }
            else{
                QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("添加成功！"),QMessageBox::Ok);
                ui->lineEdit->clear();
                ui->lineEdit_2->clear();
            }
        }
        else{
            QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请选择用户类型！"),QMessageBox::Ok);
        }
    }

}

void addaccount::on_lineEdit_textEdited()
{
    if(getRadio()==1){
        if(!numCheck(q_s(ui->lineEdit->text()),11)){
            ui->warn->setText(s_q("用户帐号为11位学号！！"));
        }
        else{
            ui->warn->setText(s_q(""));
        }
    }
    else{
        if(!numCheck(q_s(ui->lineEdit->text()),8)){
            ui->warn->setText(s_q("管理员帐号为8位工号！！"));
        }
        else{
            ui->warn->setText(s_q(""));
        }
    }
}

void addaccount::on_radioButton_clicked()
{
    ui->warn->setText(s_q("用户帐号为11位学号！！"));
}

void addaccount::on_radioButton_2_clicked()
{
    ui->warn->setText(s_q("管理员帐号为8位工号！！"));
}
