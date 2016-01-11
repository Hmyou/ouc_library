#include "index.h"
#include "ui_index.h"
#include "login.h"
#include <QString>
#include <vector>
#include <utility>
#include <QLabel>
#include "data.h"
#include <qdebug.h>
#include <QWidget.h>
#include <string>
#include <sstream>

using namespace std;
using namespace DataManagement;
using namespace BookManagement;


void index::hideTxt(){
    ui->_10->hide();ui->_11->hide();ui->_12->hide();ui->_13->hide();ui->_14->hide();ui->_15->hide();
    ui->_20->hide();ui->_21->hide();ui->_22->hide();ui->_23->hide();ui->_24->hide();ui->_25->hide();
    ui->_30->hide();ui->_31->hide();ui->_32->hide();ui->_33->hide();ui->_34->hide();ui->_35->hide();
    ui->_40->hide();ui->_41->hide();ui->_42->hide();ui->_43->hide();ui->_44->hide();ui->_45->hide();
}

void index::clearTxt(){
    ui->_10->clear();ui->_11->clear();ui->_12->clear();ui->_13->clear();ui->_14->clear();ui->_15->text().clear();
    ui->_20->clear();ui->_21->clear();ui->_22->clear();ui->_23->clear();ui->_24->clear();ui->_25->text().clear();
    ui->_30->clear();ui->_31->clear();ui->_32->clear();ui->_33->clear();ui->_34->clear();ui->_35->text().clear();
    ui->_40->clear();ui->_41->clear();ui->_42->clear();ui->_43->clear();ui->_44->clear();ui->_45->text().clear();
}

index::index(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::index)
{
    ui->setupUi(this);
    DataInit(false);
    hideTxt();
    prevNum = nextNum = -1;
    ui->warnTxt->setVisible(true);
    ui->warnTxt->setText(s_q("欢迎使用图书馆管理系统！"));
    ui->lastButton->hide();ui->nextButton->hide();ui->warnTxt_2->setText(s_q("")); 
    log = new login(this);
    log->hide();
    bookFrame = new bookinfo(this);
    connect(this,SIGNAL(send(QString)),bookFrame,SLOT(receive(QString)));
    bookFrame->hide();
}

index::~index()
{
    delete ui;
}

void index::showagain(){
    this->show();
}

void index::printBookList(DataManagement::Book **list){
    int i = 0;
    newVt.clear();
    vt.clear();   
    prevNum = nextNum;
    if(list[0] == NULL){
        ui->warnTxt->setText(s_q("没有查询到相关书籍"));
        ui->lastButton->hide();ui->nextButton->hide();ui->warnTxt_2->clear();
        return;
    }else{
        ui->warnTxt->clear();
    }

    while (list[i] != nullptr){
        vt.push_back(list[i]);
        i++;
    }
    for (auto i = vt.begin(); i != vt.end();) {
        auto j = i;
        for (; j != vt.end() && strcmp((*i)->ISBN,(*j)->ISBN)==0; j++);
        newVt.push_back(make_pair(*i, distance(i, j)));
        i = j;
    }


    int ii = 0;
    nowCnt = 0;
    while(ii < newVt.size() && ii < 4){
        int i = ii%4+1;
        string tmp1 = "_";tmp1+=char('0'+i);tmp1+="1";
        string x1;
        x1 += newVt[ii].first->Name;
        QLabel * label1 = findChild<QLabel*>(s_q(tmp1));
        label1->setText(s_q(x1));

        string tmp2 = "_";tmp2+=char('0'+i);tmp2+="2";
        string x2;
        x2+=newVt[ii].first->ISBN;
        QLabel * label2 = findChild<QLabel*>(s_q(tmp2));
        label2->setText(s_q(x2));

        string tmp3 = "_";tmp3+=char('0'+i);tmp3+="3";
        string x3 = newVt[ii].first->bookAuthor;
        QLabel * label3 = findChild<QLabel*>(s_q(tmp3));
        label3->setText(s_q(x3));

        string tmp4 = "_";tmp4+=char('0'+i);tmp4+="4";
        string x4 = newVt[ii].first->bookPublish;
        QLabel * label4 = findChild<QLabel*>(s_q(tmp4));
        label4->setText(s_q(x4));

        string tmp5 = "_";tmp5+=char('0'+i);tmp5+="5";
        string x5;
        stringstream ss;
        ss<<newVt[ii].second;
        ss>>x5;
        x5+="本馆藏";
        QPushButton * pushbotton5 = findChild<QPushButton*>(s_q(tmp5));
        pushbotton5->setText(s_q(x5));

        string tmp0 = "_";tmp0+=char('0'+i);tmp0+="0";
        if(label1->text().size()>0){
         label1->show();label2->show();label3->show();label4->show();
         findChild<QLabel*>(s_q(tmp0))->show();
         pushbotton5->setVisible(true);
        }

        ii++;
        nowCnt++;
    }
    nextNum = nowCnt;
}

int index::getRadioKind() {
    if (ui->radioBook->isChecked()) {
        return 1;
    }
    else if (ui->radioISBN->isChecked()){
        return 2;
    }
    else {
        return -1;
    }
}

void index::on_loginButton_clicked()
{
    log->show();
    this->hide();
}


void index::on_searchButton_clicked()
{
    clearTxt();
    hideTxt();
    ui->lastButton->show();ui->nextButton->show();ui->warnTxt_2->clear();
    prevNum = nextNum = -1;
    DataManagement::Book**  bookList;
    string str = q_s( ui->txtEdit->text() );
    if (str.size() == 0) {
        ui->warnTxt->setText(s_q("请输入查询关键字！"));
        ui->lastButton->hide();ui->nextButton->hide();ui->warnTxt_2->clear();
        return;
    }
    int flag = getRadioKind();
    if (flag == -1) {
        ui->warnTxt->setText(s_q("请选择查询方式"));
        ui->lastButton->hide();ui->nextButton->hide();ui->warnTxt_2->clear();
        return;
    }
    else if (flag == 1) {
        bookList = BookManagement::SelectBookApartName(str);
    }
    else if (flag == 2) {
        if(!isbnCheck(str)){
            ui->warnTxt->setText(s_q("请输入正确的ISBN号！！"));
            ui->lastButton->hide();ui->nextButton->hide();ui->warnTxt_2->clear();
            return;
        }
        else{
            bookList = DataManagement::SelectBookIsbn_internal(str);
        }
    }

    printBookList(bookList);
}

void index::on_lastButton_clicked()
{
    ui->warnTxt_2->clear();
    if (prevNum < 4) {
        ui->warnTxt_2->setText(s_q("已经是第一页了"));
        return;
    }
    clearTxt();
    hideTxt();

    nowCnt -= nextNum;
    nowCnt -= prevNum;
    if (nowCnt >= 4) prevNum = 4;
    else prevNum = nowCnt;
    int tmpi = 0;
    int pre_x = nowCnt;
    while(nowCnt < newVt.size() && tmpi < 4) {
        int i = (nowCnt % 4) + 1;

        string tmp1 = "_";tmp1+=char('0'+i);tmp1+="1";
        string x1;
        x1 += newVt[nowCnt].first->Name;
        QLabel * label1 = findChild<QLabel*>(s_q(tmp1));
        label1->setText(s_q(x1));

        string tmp2 = "_";tmp2+=char('0'+i);tmp2+="2";
        string x2;
        x2+=newVt[nowCnt].first->ISBN;
        QLabel * label2 = findChild<QLabel*>(s_q(tmp2));
        label2->setText(s_q(x2));

        string tmp3 = "_";tmp3+=char('0'+i);tmp3+="3";
        string x3 = newVt[nowCnt].first->bookAuthor;
        QLabel * label3 = findChild<QLabel*>(s_q(tmp3));
        label3->setText(s_q(x3));

        string tmp4 = "_";tmp4+=char('0'+i);tmp4+="4";
        string x4 = newVt[nowCnt].first->bookPublish;
        QLabel * label4 = findChild<QLabel*>(s_q(tmp4));
        label4->setText(s_q(x4));

        string tmp5 = "_";tmp5+=char('0'+i);tmp5+="5";
        string x5;
        stringstream ss;
        ss<<newVt[nowCnt].second;
        ss>>x5;
        x5+="本馆藏";
        QPushButton * pushbotton5 = findChild<QPushButton*>(s_q(tmp5));

        pushbotton5->setText(s_q(x5));

        string tmp0 = "_";tmp0+=char('0'+i);tmp0+="0";
        if(label1->text().size()>0){
         label1->show();label2->show();label3->show();label4->show();
         findChild<QLabel*>(s_q(tmp0))->show();
         pushbotton5->setVisible(true);
        }

        tmpi++;
        nowCnt++;
    }
    int now_x = nowCnt;
    nextNum = now_x - pre_x;
}

void index::on_nextButton_clicked()
{
    ui->warnTxt_2->clear();
    if(nowCnt >= newVt.size()){
        ui->warnTxt_2->setText(s_q("已经是最后一页了"));
        return;
    }

    prevNum = nextNum;
    clearTxt();
    hideTxt();
    int tmpi = 0;
    int pre_x = nowCnt;
    while(nowCnt < newVt.size() && tmpi < 4) {
        int i = (nowCnt % 4) + 1;
        string tmp1 = "_";tmp1+=char('0'+i);tmp1+="1";
        string x1;
        x1 += newVt[nowCnt].first->Name;
        QLabel * label1 = findChild<QLabel*>(s_q(tmp1));
        label1->setText(s_q(x1));

        string tmp2 = "_";tmp2+=char('0'+i);tmp2+="2";
        string x2;
        x2+=newVt[nowCnt].first->ISBN;
        QLabel * label2 = findChild<QLabel*>(s_q(tmp2));
        label2->setText(s_q(x2));

        string tmp3 = "_";tmp3+=char('0'+i);tmp3+="3";
        string x3 = newVt[nowCnt].first->bookAuthor;
        QLabel * label3 = findChild<QLabel*>(s_q(tmp3));
        label3->setText(s_q(x3));

        string tmp4 = "_";tmp4+=char('0'+i);tmp4+="4";
        string x4 = newVt[nowCnt].first->bookPublish;
        QLabel * label4 = findChild<QLabel*>(s_q(tmp4));
        label4->setText(s_q(x4));

        string tmp5 = "_";tmp5+=char('0'+i);tmp5+="5";
        string x5;
        stringstream ss;
        ss<<newVt[nowCnt].second;
        ss>>x5;
        x5+="本馆藏";
        QPushButton * pushbotton5 = findChild<QPushButton*>(s_q(tmp5));
        pushbotton5->setText(s_q(x5));

        string tmp0 = "_";tmp0+=char('0'+i);tmp0+="0";
        if(label1->text().size()>0){
         label1->show();label2->show();label3->show();label4->show();
         findChild<QLabel*>(s_q(tmp0))->show();
         pushbotton5->setVisible(true);
        }

        tmpi++;
        nowCnt++;
    }
    int now_x = nowCnt;
    nextNum = now_x - pre_x;
}



void index::on__15_clicked()
{
    emit send(ui->_12->text());
    bookFrame->show();
}

void index::on__25_clicked()
{
    emit send(ui->_22->text());
    bookFrame->show();
}

void index::on__35_clicked()
{
    emit send(ui->_32->text());
    bookFrame->show();
}

void index::on__45_clicked()
{
    emit send(ui->_42->text());
    bookFrame->show();
}


