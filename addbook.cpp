#include "addbook.h"
#include "ui_addbook.h"
#include <string>
#include <qmessagebox.h>

using namespace BookManagement;

addbook::addbook(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::addbook)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    setFixedSize(this->width(), this->height());
}

addbook::~addbook()
{
    delete ui;
}

void addbook::clearbookinfo(){
    ui->nameEdit->clear();
    ui->authorEdit->clear();
    ui->isbnEdit->clear();
    ui->pubEdit->clear();
    ui->priceEdit->clear();
    ui->numEdit->clear();
    ui->warn->clear();ui->warn_2->clear();ui->warn_3->clear();ui->warn_4->clear();ui->warn_5->clear();ui->warn_6->clear();
}

void addbook::on_pushButton_2_clicked()
{
    clearbookinfo();
    this->hide();
}

void addbook::on_pushButton_clicked()
{
    if(ui->isbnEdit->text().isEmpty()||ui->nameEdit->text().isEmpty()||ui->authorEdit->text().isEmpty()||ui->pubEdit->text().isEmpty()||ui->priceEdit->text().isEmpty()||ui->numEdit->text().isEmpty()){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("请补全书籍信息后再添加！！"),QMessageBox::Ok);
        return;
    }
    if(ui->warn->text()!=""||ui->warn_2->text()!=""||ui->warn_3->text()!=""||ui->warn_4->text()!=""||ui->warn_5->text()!=""||ui->warn_6->text()!=""){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("存在输入错误，请查看红色错误提示！！"),QMessageBox::Ok);
        return;
    }
    int addr = AddBook(q_s(ui->isbnEdit->text()),q_s(ui->nameEdit->text()),q_s(ui->authorEdit->text()),q_s(ui->pubEdit->text()),atof(q_s(ui->priceEdit->text()).c_str()),atoi(q_s(ui->numEdit->text()).c_str()));
    if(addr == -1){
        QMessageBox::warning(this,tr("Warning"),QString::fromLocal8Bit("所填信息与已有ISBN的书籍信息不同！！"),QMessageBox::Ok);
    }
    else{
        QMessageBox::information(this,tr("Promot"),QString::fromLocal8Bit("添加成功"),QMessageBox::Ok);
    }
}

void addbook::on_isbnEdit_textEdited()
{
    if(!isbnCheck(q_s(ui->isbnEdit->text()))){
        ui->warn->setText(s_q("请输入正确的ISBN号"));
    }
    else{
        ui->warn->setText(s_q(""));
    }
}

void addbook::on_priceEdit_textEdited()
{
    QString strPatt = "^[0-9]+(.[0-9]{2})?$";
    QRegExp rx(strPatt);
    if(!rx.exactMatch(ui->priceEdit->text())){
        ui->warn_2->setText(s_q("请输入正确价格（非负的整数或两位小数）"));
    }
    else{
        ui->warn_2->setText(s_q(""));
    }
}

void addbook::on_numEdit_textEdited()
{
    QString strPatt = "^[1-9][0-9]$|^0[1-9]$|^[1-9]$";
    QRegExp rx(strPatt);
    if(!rx.exactMatch(ui->numEdit->text())){
        ui->warn_3->setText(s_q("请输入正确数量（小于100的正整数）"));
    }
    else{
        ui->warn_3->setText(s_q(""));
    }
}

void addbook::on_nameEdit_textEdited()
{
    QString s = ui->nameEdit->text();
    string str = _trim(q_s(s));
    if (str.size() <= 0 || str.size() > BOOK_NAME_LEN){
        ui->warn_4->setText(s_q("请输入书名信息且长度小于100"));
    }
    else{
        ui->warn_4->setText(s_q(""));
    }
}

void addbook::on_authorEdit_textEdited()
{
    QString s = ui->authorEdit->text();
    string str = _trim(q_s(s));
    if (str.size() <= 0 || str.size() > BOOK_AUTHOR_LEN){
        ui->warn_5->setText(s_q("请输入作者信息且长度小于50"));
    }
    else{
        ui->warn_5->setText(s_q(""));
    }
}

void addbook::on_pubEdit_textEdited()
{
    QString s = ui->pubEdit->text();
    string str = _trim(q_s(s));
    if (str.size() <= 0 || str.size() > BOOK_PUBLISHER_LEN){
        ui->warn_6->setText(s_q("请输入出版社信息且长度小于50"));
    }
    else{
        ui->warn_6->setText(s_q(""));
    }
}
