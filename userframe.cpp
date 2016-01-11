#include "userframe.h"
#include "ui_userframe.h"
#include <qmessagebox.h>
#include <qdatetime.h>
#include <string>
#include <sstream>

using namespace std;
using namespace UserManagement;
using namespace BookManagement;


userframe::userframe(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userframe)
{
    ui->setupUi(this);
    connect(this,SIGNAL(exittoindex()),parent,SLOT(showagain()));
}

userframe::~userframe()
{
    delete ui;
}

void userframe::receive(QString str){
    accountReceive = str;
    ui->labelAccount->setText(accountReceive);
    ui->labelAccount->adjustSize();
    cleartxt();
    overLimit = false;
    User *tmpuser = SelectUser(q_s(ui->labelAccount->text()));
    if(IsTimeLimit(tmpuser->ID)){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("亲，你存在超期书籍，续借功能已不可用。进入界面后请查看，请尽快还书！"),QMessageBox::Yes);
        overLimit = true;
        ui->_15->hide();ui->_25->hide();ui->_35->hide();ui->_45->hide();
    }

    Record **record = SelectRecordUserID(tmpuser->ID);
    printList(record);
    delete tmpuser;
}

void userframe::on_exitButton_clicked()
{
    emit exittoindex();
    reject();
}

void userframe::on_changeButton_clicked()
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

void userframe::cleartxt(){
    ui->_11->clear();ui->_12->clear();ui->_13->clear();ui->_14->clear();ui->_15->hide();
    ui->_21->clear();ui->_22->clear();ui->_23->clear();ui->_24->clear();ui->_25->hide();
    ui->_31->clear();ui->_32->clear();ui->_33->clear();ui->_34->clear();ui->_35->hide();
    ui->_41->clear();ui->_42->clear();ui->_43->clear();ui->_44->clear();ui->_45->hide();
}

void userframe::printList(Record **list){
    cleartxt();
    newVt.clear();
    vt.clear();
    prevNum = nextNum = -1;
    if(list[0] == NULL){
        return;
    }
    int i = 0;
    if(list[0] == NULL){
        return;
    }

    while (list[i] != nullptr){
        vt.push_back(list[i]);
        i++;
    }
    for (auto i = vt.begin(); i != vt.end(); i++)
    {
        newVt.push_back(make_pair(*i, 1));
    }

    int ii = 0;
    nowCnt = 0;
    while(ii < newVt.size() && ii < 4){
        i = ii%4+1;

        string tmp1 = "_";tmp1+=char('0'+i);tmp1+="1";
        string x1="";
        int tid=newVt[ii].first->bookId;
        Book *thisBook = SelectID<Book>(tid);
        x1+=thisBook->Name;
        QLabel *label1 = findChild<QLabel*>(s_q(tmp1));
        label1->setText(s_q(x1));

        string tmp2 = "_";tmp2+=char('0'+i);tmp2+="2";
        char buff[128];
        sprintf(buff, "%010d", thisBook->ID);
        string x2 = buff;
        QLabel *label2 = findChild<QLabel*>(s_q(tmp2));
        label2->setText(s_q(x2));

        string tmp3 = "_";tmp3+=char('0'+i);tmp3+="3";
        QString x3 = QDateTime::fromTime_t(newVt[ii].first->borrowTime).toUTC().toString("yyyy-MM-dd");
        QLabel *label3 = findChild<QLabel*>(s_q(tmp3));
        label3->setText(x3);

        string tmp4 = "_";tmp4+=char('0'+i);tmp4+="4";
        QLabel *label4 = findChild<QLabel*>(s_q(tmp4));
        int rid = newVt[ii].first->ID;
        cmpTimeRecord = SelectID<Record>(rid);
        if(cmpTimeRecord->borrowTime == cmpTimeRecord->returnTime){
            if(!cmpTimeRecord->isRenewed){
                label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime+MAX_BORROW_TIME*24*60*60).toUTC().toString("yyyy-MM-dd"));
            }
            else{
                label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
            }
        }
        else{
            label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime).toUTC().toString("yyyy-MM-dd"));
        }

        string tmp5 = "_";tmp5+=char('0'+i);tmp5+="5";
        QPushButton *button4 = findChild<QPushButton*>(s_q(tmp5));
        if(cmpTimeRecord->isBorrowed && !cmpTimeRecord->isRenewed){
            button4->setEnabled(true);
            button4->show();
        }
        else{
            button4->setEnabled(false);
            button4->hide();
        }

        delete cmpTimeRecord;cmpTimeRecord = nullptr;
        ii++;
        nowCnt++;
    }
    nextNum = nowCnt;
    if(overLimit==true){
        ui->_15->hide();ui->_25->hide();ui->_35->hide();ui->_45->hide();
    }
}

void userframe::on_lastButton_clicked()
{
    if (prevNum < 4) {
        ui->warn->setText(s_q("已经是第一页了"));
        return;
    }
    cleartxt();
    nowCnt -= nextNum;
    nowCnt -= prevNum;
    if (nowCnt >= 4) prevNum = 4;
        else prevNum = nowCnt;
    int ii = 0;
    int pre_x = nowCnt;
    int i = 0;
    while(nowCnt < newVt.size() && ii < 4) {
        i = nowCnt%4+1;

        string tmp1 = "_";tmp1+=char('0'+i);tmp1+="1";
        string x1="";
        int tid=newVt[nowCnt].first->bookId;
        Book *thisBook = SelectID<Book>(tid);
        x1+=thisBook->Name;
        QLabel *label1 = findChild<QLabel*>(s_q(tmp1));
        label1->setText(s_q(x1));

        string tmp2 = "_";tmp2+=char('0'+i);tmp2+="2";
        char buff[128];
        sprintf(buff, "%010d", thisBook->ID);
        string x2 = buff;
        QLabel *label2 = findChild<QLabel*>(s_q(tmp2));
        label2->setText(s_q(x2));

        string tmp3 = "_";tmp3+=char('0'+i);tmp3+="3";
        QString x3 = QDateTime::fromTime_t(newVt[nowCnt].first->borrowTime).toUTC().toString("yyyy-MM-dd");
        QLabel *label3 = findChild<QLabel*>(s_q(tmp3));
        label3->setText(x3);

        string tmp4 = "_";tmp4+=char('0'+i);tmp4+="4";
        QLabel *label4 = findChild<QLabel*>(s_q(tmp4));
        int rid = newVt[nowCnt].first->ID;
        cmpTimeRecord = SelectID<Record>(rid);
        if(cmpTimeRecord->borrowTime == cmpTimeRecord->returnTime){
            if(!cmpTimeRecord->isRenewed){
                label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime+MAX_BORROW_TIME*24*60*60).toUTC().toString("yyyy-MM-dd"));
            }
            else{
                label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
            }
        }
        else{
            label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime).toUTC().toString("yyyy-MM-dd"));
        }

        string tmp5 = "_";tmp5+=char('0'+i);tmp5+="5";
        QPushButton *button4 = findChild<QPushButton*>(s_q(tmp5));
        if(cmpTimeRecord->isBorrowed && !cmpTimeRecord->isRenewed){
            button4->setEnabled(true);
            button4->show();
        }
        else{
            button4->setEnabled(false);
            button4->hide();
        }

        delete cmpTimeRecord;cmpTimeRecord = nullptr;
        ii++;
        nowCnt++;
    }
    nextNum = nowCnt - pre_x;
    if(overLimit==true){
        ui->_15->hide();ui->_25->hide();ui->_35->hide();ui->_45->hide();
    }
}

void userframe::on_nextButton_clicked()
{
    if(nowCnt >= newVt.size()){
        ui->warn->setText(s_q("已经是最后一页了"));
        return;
    }
    cleartxt();
    prevNum = nextNum;
    int ii = 0;
    int pre_x = nowCnt;
    int i = 0;
    while(nowCnt < newVt.size() && ii < 4) {
        i = nowCnt%4+1;

        string tmp1 = "_";tmp1+=char('0'+i);tmp1+="1";
        string x1="";
        int tid=newVt[nowCnt].first->bookId;
        Book *thisBook = SelectID<Book>(tid);
        x1+=thisBook->Name;
        QLabel *label1 = findChild<QLabel*>(s_q(tmp1));
        label1->setText(s_q(x1));

        string tmp2 = "_";tmp2+=char('0'+i);tmp2+="2";
        char buff[128];
        sprintf(buff, "%010d", thisBook->ID);
        string x2 = buff;
        QLabel *label2 = findChild<QLabel*>(s_q(tmp2));
        label2->setText(s_q(x2));

        string tmp3 = "_";tmp3+=char('0'+i);tmp3+="3";
        QString x3 = QDateTime::fromTime_t(newVt[nowCnt].first->borrowTime).toUTC().toString("yyyy-MM-dd");
        QLabel *label3 = findChild<QLabel*>(s_q(tmp3));
        label3->setText(x3);

        string tmp4 = "_";tmp4+=char('0'+i);tmp4+="4";
        QLabel *label4 = findChild<QLabel*>(s_q(tmp4));
        int rid = newVt[nowCnt].first->ID;
        cmpTimeRecord = SelectID<Record>(rid);
        if(cmpTimeRecord->borrowTime == cmpTimeRecord->returnTime){
            if(!cmpTimeRecord->isRenewed){
                label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime+MAX_BORROW_TIME*24*60*60).toUTC().toString("yyyy-MM-dd"));
            }
            else{
                label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
            }
        }
        else{
            label4->setText(QDateTime::fromTime_t(cmpTimeRecord->returnTime).toUTC().toString("yyyy-MM-dd"));
        }

        string tmp5 = "_";tmp5+=char('0'+i);tmp5+="5";
        QPushButton *button4 = findChild<QPushButton*>(s_q(tmp5));
        if(cmpTimeRecord->isBorrowed && !cmpTimeRecord->isRenewed){
            button4->setEnabled(true);
            button4->show();
        }
        else{
            button4->setEnabled(false);
            button4->hide();
        }
        delete cmpTimeRecord;cmpTimeRecord = nullptr;
        ii++;
        nowCnt++;
    }
    nextNum = nowCnt - pre_x;
    if(overLimit==true){
        ui->_15->hide();ui->_25->hide();ui->_35->hide();ui->_45->hide();
    }
}

void userframe::on__15_clicked()
{
    User *tmpuser = SelectUser(q_s(ui->labelAccount->text()));
    string bID = q_s(ui->_12->text());
    int intbID = atoi(bID.c_str());
    ReBorrowBook(tmpuser->ID,intbID);
    Record **reTime = SelectRecordTwoID(tmpuser->ID,intbID);
    int i=0;
    for(;reTime[i]!=nullptr,!reTime[i]->isBorrowed;i++){
        ;
    }
    ui->_14->setText(QDateTime::fromTime_t(reTime[i]->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
    ui->_15->setVisible(false);
    ui->_15->setEnabled(false);
    delete tmpuser;tmpuser=nullptr;
    for(i=0;reTime[i]!=nullptr;i++){
        delete reTime[i];reTime[i]=nullptr;
    }
}

void userframe::on__25_clicked()
{
    User *tmpuser = SelectUser(q_s(ui->labelAccount->text()));
    string bID = q_s(ui->_22->text());
    int intbID = atoi(bID.c_str());
    ReBorrowBook(tmpuser->ID,intbID);
    Record **reTime = SelectRecordTwoID(tmpuser->ID,intbID);
    int i=0;
    for(;reTime[i]!=nullptr,!reTime[i]->isBorrowed;i++){
        ;
    }
    ui->_24->setText(QDateTime::fromTime_t(reTime[i]->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
    ui->_25->setVisible(false);
    ui->_25->setEnabled(false);
    delete tmpuser;tmpuser=nullptr;
    for(i=0;reTime[i]!=nullptr;i++){
        delete reTime[i];reTime[i]=nullptr;
    }
}

void userframe::on__35_clicked()
{
    User *tmpuser = SelectUser(q_s(ui->labelAccount->text()));
    string bID = q_s(ui->_32->text());
    int intbID = atoi(bID.c_str());
    ReBorrowBook(tmpuser->ID,intbID);
    Record **reTime = SelectRecordTwoID(tmpuser->ID,intbID);
    int i=0;
    for(;reTime[i]!=nullptr,!reTime[i]->isBorrowed;i++){
        ;
    }
    ui->_34->setText(QDateTime::fromTime_t(reTime[i]->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
    ui->_35->setVisible(false);
    ui->_35->setEnabled(false);
    delete tmpuser;tmpuser=nullptr;
    for(i=0;reTime[i]!=nullptr;i++){
        delete reTime[i];reTime[i]=nullptr;
    }
}

void userframe::on__45_clicked()
{
    User *tmpuser = SelectUser(q_s(ui->labelAccount->text()));
    string bID = q_s(ui->_42->text());
    int intbID = atoi(bID.c_str());
    ReBorrowBook(tmpuser->ID,intbID);
    Record **reTime = SelectRecordTwoID(tmpuser->ID,intbID);
    int i=0;
    for(;reTime[i]!=nullptr,!reTime[i]->isBorrowed;i++){
        ;
    }
    ui->_44->setText(QDateTime::fromTime_t(reTime[i]->returnTime+MAX_BORROW_TIME*2*24*60*60).toUTC().toString("yyyy-MM-dd"));
    ui->_45->setVisible(false);
    ui->_45->setEnabled(false);
    delete tmpuser;tmpuser=nullptr;
    for(i=0;reTime[i]!=nullptr;i++){
        delete reTime[i];reTime[i]=nullptr;
    }
}
