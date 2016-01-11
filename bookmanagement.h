#ifndef BOOKMANAGEMENT_H
#define BOOKMANAGEMENT_H

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include "data.h"
using namespace DataManagement;
using namespace std;

namespace BookManagement{
    //保存book信息文件是否存在
    bool RecordIsExist();

    //保存book信息文件是否存在
    bool BookIsExist();


    //用户的某本借书记录,可能有多条,因为记录不会删除
    Record** SelectRecordTwoID(int userID, int bookID);

    //用户的借书记录,多条
    Record** SelectRecordUserID(int userID);

    //Isbn查找Book,可能有好几本
    Book** SelectBookIsbn(string Isbn);

    //书名找book,可能有好几本
    Book** SelectBook(string Name);

    //book书名模糊查询
    Book** SelectBookApartName(string name);

    //借阅图书, 1成功
    // 其余失败 -1 无此用户 , -2 无此书籍, -3 书籍不可借, -4 已达到最大结束数量12
    int  BorrowBook(string userName, int ID);

    //续借图书
    void ReBorrowBook(int userID, int bookID);

    //归还图书
    //1正常还书,0超期还书,-1还书失败
    int ReturnBook(string userName, int ID);


    //增加图书
    //最后一个属性是要增加书的数量
    int AddBook(string Isbn, string Name, string Author, string Publisher, double price,int nums);

    //删除图书 成功1 书不存在-1 书正被借出0
    int DeleteBook(int ID);


    //是否存在超期图书 true有超期图书
    bool IsTimeLimit(int userID);
}

#endif // BOOKMANAGEMENT_H
