#include "adminframe.h"
#include "ui_adminframe.h"
#include <qmessagebox.h>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <qdebug.h>

using namespace std;
using namespace DataManagement;
using namespace UserManagement;
using namespace BookManagement;

void adminframe::hidetxtbtn(){
    ui->label_2->hide();ui->label_3->hide();
    ui->resultAccount->hide();ui->resultKind->hide();
    ui->changeButton_2->hide();ui->newpassEdit_2->hide();
    ui->deleteButton->hide();
    ui->newpassEdit->clear();ui->newpassEdit_2->clear();
}
void adminframe::showtxtbtn(){
    ui->label_2->show();ui->label_3->show();
    ui->resultAccount->show();ui->resultKind->show();
    ui->changeButton_2->show();ui->newpassEdit_2->show();
    ui->deleteButton->show();
}

adminframe::adminframe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::adminframe)
{
    addAccountFrame = new addaccount();
    addAccountFrame->hide();
    addBookFrame = new addbook();
    addBookFrame->hide();
    ui->setupUi(this);
    hidetxtbtn();
    connect(this,SIGNAL(exittoindex()),parent,SLOT(showagain()));
    resultHide();
    flag = -1;
}

adminframe::~adminframe()
{
    delete ui;
}

void adminframe::showagain(){
    this->show();
}

void adminframe::receive(QString str){
    accountReceive = str;
    ui->labelAccount->setText(accountReceive);
    ui->labelAccount->adjustSize();
}

void adminframe::on_exitButton_clicked()
{
    hidetxtbtn();
    resultHide();
    ui->warn->clear();ui->searchbook->clear();ui->searchbook->show();ui->warn->show();
    ui->pushButton->show();ui->pushButton_2->hide();
    emit exittoindex();
    reject();
}

void adminframe::on_adduserButton_clicked()
{

    addAccountFrame->show();
}

void adminframe::on_changeButton_clicked()
{
    if(ui->newpassEdit->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请输入所要修改的密码！"),QMessageBox::Ok);
    }
    else{
        if(UpdataUserPassword(q_s(ui->labelAccount->text()),q_s(ui->newpassEdit->text()))){
            QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("修改成功"),QMessageBox::Ok);
            ui->newpassEdit->clear();
        }
        else{
            QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("修改成功"),QMessageBox::Ok);
            ui->newpassEdit->clear();
        }
    }
}

void adminframe::on_searchButton_clicked()
{
    hidetxtbtn();
    if(ui->searchEdit->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请输入所要查询内容！"),QMessageBox::Ok);
    }
    else{

            User *resultuser = SelectUser(q_s(ui->searchEdit->text()));
            if(resultuser!=nullptr){
                ui->resultAccount->setText(s_q(resultuser->Name));
                if(!strcmp(q_s(resultuser->type).c_str(),"admin")){
                    ui->resultKind->setText(s_q("管理员"));
                }
                else if(!strcmp(q_s(resultuser->type).c_str(),"user")){
                    ui->resultKind->setText(s_q("用户"));
                }
                else{
                    ui->resultKind->clear();
                }
                showtxtbtn();
            }
            else{
                QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("不存在该用户"),QMessageBox::Ok);
                hidetxtbtn();
                ui->searchEdit->clear();
            }
            ui->searchEdit->clear();

    }
}

void adminframe::on_changeButton_2_clicked()
{
    if(ui->newpassEdit_2->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请输入所要修改的密码！"),QMessageBox::Ok);
    }
    else{
        if(UpdataUserPassword(q_s(ui->resultAccount->text()),q_s(ui->newpassEdit_2->text()))){
            QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("修改成功"),QMessageBox::Ok);
            ui->newpassEdit_2->clear();
        }
        else{
            QMessageBox::information(this,tr("Prompt"),QString::fromLocal8Bit("修改失败"),QMessageBox::Ok);
            ui->newpassEdit_2->clear();
        }
    }
}

void adminframe::on_deleteButton_clicked()
{
    if(!strcmp(q_s(ui->resultAccount->text()).c_str(),"root")){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("还想玩不，系统总管理员不允许删除！"),QMessageBox::Ok);
    }
    else if(!strcmp(q_s(ui->labelAccount->text()).c_str(),q_s(ui->resultAccount->text()).c_str())){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("不允许自己删自己啊~亲"),QMessageBox::Ok);
    }
    else{
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Delete user", QString::fromLocal8Bit("确认删除用户？"),QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            int deleteresult = UserManagement::DeleteUser(q_s(ui->resultAccount->text()));
            if (deleteresult == -1) {
                QMessageBox::warning(this,tr("Faile"),QString::fromLocal8Bit("该用户有在借书籍！删除失败，请督促还书！"),QMessageBox::Yes);
            }else if (deleteresult == 1){
                QMessageBox::information(this,tr("Success"),QString::fromLocal8Bit("删除成功"),QMessageBox::Yes);
                hidetxtbtn();
            }
            else{
                QMessageBox::warning(this,tr("Fail"),QString::fromLocal8Bit("删除失败"),QMessageBox::Yes);
            }
        }
        else{}
    }

}

void adminframe::on_addbookButton_clicked()
{
    addBookFrame->show();
}

void adminframe::bottomShow(){
    ui->bottom2->clear();
    ui->bottom1->show();
    ui->bottom2->show();
    ui->bottom3->show();
    ui->bottom4->show();
}

void adminframe::bottomHide(){
    ui->bottom2->clear();
    ui->bottom1->hide();
    ui->bottom2->hide();
    ui->bottom3->hide();
    ui->bottom4->hide();
}

void adminframe::resultHide(){
    ui->result1->clear();ui->result2->clear();ui->result3->clear();ui->result4->clear();
    ui->result1->hide();ui->result2->hide();ui->result3->hide();ui->result4->hide();
    ui->result01->hide();ui->result02->hide();ui->result03->hide();ui->result04->hide();
    ui->borrowButton->hide();ui->returnButton->hide();ui->deleteBook->hide();
    bottomHide();
}

void adminframe::resultShow(){
    ui->result1->show();ui->result2->show();ui->result3->show();ui->result4->show();
    ui->result01->show();ui->result02->show();ui->result03->show();ui->result04->show();
    ui->borrowButton->show();ui->returnButton->show();ui->deleteBook->show();
}

void adminframe::on_pushButton_clicked()
{


        ui->returnButton->setStyleSheet("border-radius:5px;background:#fe7023;color:#ffffff;font: 10pt");
        ui->borrowButton->setStyleSheet("border-radius:5px;background:#fe7023;color:#ffffff;font: 10pt");
        ui->deleteBook->setStyleSheet("border-radius:5px;background:#fe7023;color:#ffffff;font: 10pt");
        ui->returnButton->setEnabled(true);ui->borrowButton->setEnabled(true);ui->deleteBook->setEnabled(true);
        resultHide();
        ui->warn->clear();
        if(ui->searchbook->text().isEmpty()){
            QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请输入查询内容！"),QMessageBox::Ok);
        }
        else{
            QString strPatt = "^\\d+$";
            QRegExp rx(strPatt);
            if(!rx.exactMatch(ui->searchbook->text())){
                ui->warn->setText(s_q("请输入正确的索书号（非负整数）！"));
            }
            else{
                string str = q_s(ui->searchbook->text());
                searchID = atoi(str.c_str());
                Book* tBook = SelectID<Book>(searchID);
                if(tBook->ID == -1){
                    ui->warn->setText(s_q("不存在此索书号书籍，请返回主界面查询确定"));
                }
                else{
                    if(tBook->isDeleted){
                        ui->warn->setText(s_q("不存在此索书号书籍，请返回主界面查询确定"));
                    }
                    else{
                        Book *tb = SelectID<Book>(searchID);
                        Book **bl = SelectBookIsbn(q_s(tb->ISBN));
                        ui->result1->setText(s_q(tBook->Name));
                        ui->result2->setText(s_q(tBook->bookAuthor));
                        ui->result3->setText(s_q(tBook->bookPublish));
                        char buff[100];
                        sprintf(buff, "%010d", tBook->ID);
                        string str = buff;
                        ui->result4->setText(s_q(str));
                        if(bl[0]->ID == tb->ID){
                            ui->warn->setText(s_q("提示：该书为保存本，不能借阅！"));
                        }
                        else{
                            ui->warn->clear();
                        }
                        ui->searchbook->hide();ui->pushButton->hide();ui->pushButton_2->show();
                        resultShow();
                        if(tBook->canBorrow){
                            ui->returnButton->setEnabled(false);
                            ui->returnButton->setStyleSheet("border-radius:5px;background:#d5d5d5;color:#ffffff;font: 10pt");
                        }
                        else{
                            if(q_s(ui->warn->text())=="提示：该书为保存本，不能借阅！"){
                                ui->returnButton->setEnabled(false);
                                ui->returnButton->setStyleSheet("border-radius:5px;background:#d5d5d5;color:#ffffff;font: 10pt");
                            }
                            ui->borrowButton->setEnabled(false);ui->deleteBook->setEnabled(false);
                            ui->borrowButton->setStyleSheet("border-radius:5px;background:#d5d5d5;color:#ffffff;font: 10pt");
                            ui->deleteBook->setStyleSheet("border-radius:5px;background:#d5d5d5;color:#ffffff;font: 10pt");
                        }
                    }
                }
            }

        }

}

void adminframe::on_borrowButton_clicked()
{
    flag = 1;
    bottomShow();
}

void adminframe::on_bottom3_clicked()
{
    if(ui->bottom2->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请输入用户帐号！"),QMessageBox::Yes);
        return;
    }
    User *ifMUser = SelectUser(q_s(ui->bottom2->text()));
    if (ifMUser==nullptr){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("没有此用户！"),QMessageBox::Yes);
        ui->bottom2->clear();
        return;
    }
    if(!strcmp(ifMUser->type,"admin")){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("输入信息为管理员帐号，不能进行此操作！"),QMessageBox::Yes);
        ui->bottom2->clear();
        return;
    }
    delete ifMUser;

        if(flag==1){
            string str = q_s(ui->searchbook->text());
            searchID = atoi(str.c_str());
            int borrowresult = BorrowBook(q_s(ui->bottom2->text()),searchID);
            if(borrowresult == -1){
                QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("没有此用户！"),QMessageBox::Yes);
                ui->bottom2->clear();
            }
            else if(borrowresult == -2){
                QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("没有此书籍！"),QMessageBox::Yes);
            }
            else if(borrowresult == -3){
                Book *tb = SelectID<Book>(searchID);
                Book **bl = SelectBookIsbn(q_s(tb->ISBN));
                if(bl[0]->ID == tb->ID){
                    QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("已提示该本书为保存本了还要借？借书失败"),QMessageBox::Yes);
                }
                else
                    QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("该本书未被归还，借书失败"),QMessageBox::Yes);
            }
            else if(borrowresult == -4){
                QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("借书失败！你已达到最大借书量12本，请尽快归还在借书籍！"),QMessageBox::Yes);
            }
            else if(borrowresult == 1){
                QMessageBox::information(this,tr("Success"),QString::fromLocal8Bit("借书成功"),QMessageBox::Yes);
                resultHide();
            }
            else{
                QMessageBox::warning(this,tr("Fail"),QString::fromLocal8Bit("借书失败"),QMessageBox::Yes);
            }
        }
        else if(flag == 2){
            string str = q_s(ui->searchbook->text());
            searchID = atoi(str.c_str());
            int returnresult = ReturnBook(q_s(ui->bottom2->text()),searchID);
            if(returnresult == 1){
                QMessageBox::information(this,tr("Success"),QString::fromLocal8Bit("还书成功"),QMessageBox::Yes);
                resultHide();
            }
            else if(returnresult == 0){
                QMessageBox::information(this,tr("Promot"),QString::fromLocal8Bit("超期还书！请交纳相关费用"),QMessageBox::Yes);
                resultHide();
            }
            else if(returnresult == -2){
                QMessageBox::warning(this,tr("Fail"),QString::fromLocal8Bit("没有此用户！"),QMessageBox::Yes);
            }
            else{
                QMessageBox::warning(this,tr("Fail"),QString::fromLocal8Bit("还书失败,亲你没有借这本书吧"),QMessageBox::Yes);
            }
        }
        else{
            QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("错误操作！"),QMessageBox::Yes);
        }


}

void adminframe::on_bottom4_clicked()
{
    bottomHide();
}

void adminframe::on_returnButton_clicked()
{
    flag = 2;
    bottomShow();
}

void adminframe::on_deleteBook_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete user", QString::fromLocal8Bit("确认删除？"),QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        string str = q_s(ui->result4->text());
        searchID = atoi(str.c_str());
        int deleteresult = DeleteBook(searchID);
        if(deleteresult == 1){
            QMessageBox::information(this,tr("Success"),QString::fromLocal8Bit("删除成功"),QMessageBox::Yes);
            resultHide();
        }
        else if(deleteresult == 0){
            Book *tb = SelectID<Book>(searchID);
            Book **bl = SelectBookIsbn(q_s(tb->ISBN));
            if(bl[0]->ID == tb->ID){
                QMessageBox::warning(this,tr("Fail"),QString::fromLocal8Bit("该本为保存本，无法删除"),QMessageBox::Yes);
            }
            else{
                QMessageBox::information(this,tr("Fail"),QString::fromLocal8Bit("书籍正被借出，无法删除"),QMessageBox::Yes);
            }

        }
        else if(deleteresult == -1){
            QMessageBox::information(this,tr("Fail"),QString::fromLocal8Bit("该书籍不存在，删除操作无效"),QMessageBox::Yes);
        }
    }
    else{}

}

void adminframe::on_pushButton_2_clicked()
{
    ui->pushButton_2->hide();ui->searchbook->clear();ui->searchbook->show();ui->pushButton->show();
    resultHide();
}

