#include "bookinfo.h"
#include "ui_bookinfo.h"
#include <time.h>
#include <string>
#include <sstream>

using namespace std;
using namespace DataManagement;
using namespace BookManagement;

bookinfo::bookinfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::bookinfo)
{
    ui->setupUi(this);
    ui->table->setSelectionBehavior ( QAbstractItemView::SelectRows);
    ui->table->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    ui->table->clearContents();
    ui->table_2->setSelectionBehavior ( QAbstractItemView::SelectRows);
    ui->table_2->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    ui->table_2->clearContents();
}

bookinfo::~bookinfo()
{
    delete ui;
}

void bookinfo::receive(QString bISBN){
    ui->infoISBN->setText(bISBN);
    Book **tempbook = SelectBookIsbn(q_s(bISBN));
    ui->infoName->setText(s_q(tempbook[0]->Name));
    ui->infoAuthor->setText(s_q(tempbook[0]->bookAuthor));
    ui->infoPub->setText(s_q(tempbook[0]->bookPublish));
    string price_string;
    stringstream ss;
    ss<<tempbook[0]->price;
    ss>>price_string;
    ui->infoPrice->setText(s_q(price_string));
    tabelshow();
    tabelshow2();
}

void bookinfo::tabelshow(){
    while (ui->table->rowCount() > 0) {
        ui->table->removeRow(0);
    }
    Book**  bookList  = SelectBookIsbn(q_s(ui->infoISBN->text()));
    if (bookList == nullptr ) return;
    int i = 0;
    while(bookList[i] != nullptr) {
        if(!bookList[i]->isDeleted){
            showRecord(bookList[i],i);
        }
        i++;
    }
}

void bookinfo::showRecord(Book *tbook,int i){
    ui->table->insertRow(ui->table->rowCount());
    char buff[128];
    sprintf(buff, "%010d", tbook->ID);
    string str = buff;
    ui->table->setItem(ui->table->rowCount()-1, 0, new QTableWidgetItem(s_q(str)));
    string state;
    if (i == 0) state = "馆藏本";
    else if (tbook->canBorrow) state = "在馆";
    else state = "被借出";
    ui->table->setItem(ui->table->rowCount()-1, 1, new QTableWidgetItem(s_q(state)));
}

void bookinfo::tabelshow2(){
    while (ui->table_2->rowCount() > 0)
    {
        ui->table_2->removeRow(0);
    }
    Book**  bookList  = SelectBookIsbn(q_s(ui->infoISBN->text()));
    if (bookList == nullptr ) return;
    int i = 0;
    while(bookList[i] != nullptr) {
        showRecord2(bookList[i]);
        i++;
    }
}

void bookinfo::showRecord2(Book *tbook){
    int aBookId = tbook->ID;
    DataManagement::Record** record = DataManagement::SelectBookRecord_internal(aBookId);
    if (!record[0]) return;
    int i = 0;
    char buff[100];
    sprintf(buff, "%010d", aBookId);
    string str = buff;
    while(record[i] != nullptr){
        ui->table_2->insertRow( ui->table_2->rowCount() );
        ui->table_2->setItem(ui->table_2->rowCount() - 1, 0, new QTableWidgetItem(s_q(str)));
        ui->table_2->setItem(ui->table_2->rowCount() - 1, 1, new QTableWidgetItem(s_q(ctime(&record[i]->borrowTime))));

        if (record[i]->borrowTime != record[i]->returnTime){
            ui->table_2->setItem(ui->table_2->rowCount() - 1, 2, new QTableWidgetItem(s_q(ctime(&record[i]->returnTime))));
        }
        if (record[i]->isBorrowed) {
            ui->table_2->setItem(ui->table_2->rowCount() - 1, 3, new QTableWidgetItem(s_q("在借")));
        }else {
            ui->table_2->setItem(ui->table_2->rowCount()-1, 3, new QTableWidgetItem(s_q("已还")));
        }
        i++;
    }
}
