#include "bookmanagement.h"
#include <time.h>

using namespace std;

namespace BookManagement{
    //保存book信息文件是否存在
    bool RecordIsExist() {
        return FileIsExist_internal<Record>();
    }

    //保存book信息文件是否存在
    bool BookIsExist() {
        return FileIsExist_internal<Book>();
    }


    //用户的某本借书记录,可能有多条,因为记录不会删除
    Record** SelectRecordTwoID(int userID, int bookID) {
        return DataManagement::SelectRecordTwoID_internal(userID, bookID);
    }

    //用户的借书记录,多条
    Record** SelectRecordUserID(int userID) {
        return DataManagement::SelectRecordUserID_internal(userID);
    }

    //Isbn查找Book,可能有好几本
    Book** SelectBookIsbn(string Isbn)
    {
        return DataManagement::SelectBookIsbn_internal(Isbn);
    }

    //书名找book,可能有好几本
    Book** SelectBook(string Name)
    {
        return SelectNameTous<Book>(Name);
    }

    //book书名模糊查询
        Book** SelectBookApartName(string name) {
            auto temp = list<Book*>();
            auto file = OpenFile<Book>();
            auto size = sizeof Book;
            Book* item = new Book();
            fseek(file, 0, SEEK_END);
            int endID = ftell(file) / size;
            for (int i = 0; i < endID; i++) {
                fseek(file, i *size, SEEK_SET);
                fread(item, size, 1, file);
                if(!item->isDeleted && strstr(item->Name, &name.c_str()[0])) {
                    Book* t = new Book();
                    memcpy(t, item, sizeof Book);
                    temp.push_back(t);
                }
            }
            delete item;
            auto result = new Book*[temp.size() + 1];
            int j = 0;
            for (auto it = temp.begin(); it != temp.end(); it++) {
                result[j] = *it;
                j++;
            }
            result[temp.size()] = NULL;
            return result;
        }

    //借阅图书, 1成功
    // 其余失败 -1 无此用户 , -2 无此书籍, -3 书籍不可借, -4 已达到最大结束数量12
    int  BorrowBook(string userName, int ID)
    {
        User *user = SelectNameOne<User>(userName);
        Book *book = SelectID<Book>(ID);
        if (user != NULL )
        {
            if (book->ID != -1) {
                if (book->canBorrow) {
                    if (user->borrowMax > 0) {
                        user->borrowMax--;
                        InsertOrUpdate(user);
                        book->canBorrow = false;
                        InsertOrUpdate(book);
                        Record *record = Create<Record>();
                        record->userId = user->ID;
                        record->bookId = book->ID;
                        record->borrowTime = time(0);
                        record->returnTime = time(0);// 在借阅时 borrowTime = returnTime
                        record->isBorrowed = true;
                        record->isRenewed = false;
                        InsertOrUpdate(record);
                        delete user;
                        delete record;
                        delete book;
                        record = NULL;
                        book = NULL;
                        user = NULL;
                        return 1;
                    }
                    return -4;
                }
                return -3;
            }
            else
                return -2;
        }
        return -1;
    }

    //续借图书
    void ReBorrowBook(int userID, int bookID)
    {
        Record **record = SelectRecordTwoID(userID, bookID);
        for (int i = 0; record[i] != nullptr; i++){
            if (record[i]->isBorrowed == true) {
                record[i]->isRenewed = true;
                InsertOrUpdate(record[i]);
                return;
            }
        }
    }


    //是否存在超期图书 true有超期图书
    bool IsTimeLimit(int userID){
        Record **record  = SelectRecordUserID(userID);
        time_t timeNow = time(0);
        for (int i = 0; record[i] != nullptr; i++){
            if (record[i]->isBorrowed == true) {
                int Time = (int)(timeNow - record[i]->borrowTime);
                int Day = Time / (60 * 60 * 24);
                int BD = MAX_BORROW_TIME;//借书期限
                if (record[i]->isRenewed == true)
                    BD = 2 * MAX_BORROW_TIME;
                if (Day > BD)
                    return true;
                else
                    return false;
            }
        }
         return false;
    }



    //归还图书
    //1正常还书,0超期还书,-1还书失败 -2没有此用户
    int ReturnBook(string userName, int ID)
    {
        User *user = SelectNameOne<User>(userName);
        Book *book = SelectID<Book>(ID);
        if(user==NULL){
            return -2;
        }
        if ( book == NULL)
            return -1;
        Record **record = SelectRecordTwoID(user->ID, book->ID);

        for (int i = 0; record[i] != NULL; i++) {
            if (record[i]->isBorrowed == true){
                user->borrowMax++;
                InsertOrUpdate(user);
                delete user;
                user = NULL;
                record[i]->isBorrowed = false;
                record[i]->returnTime = time(0);
                book->canBorrow = true;
                InsertOrUpdate(book);
                book = nullptr;
                InsertOrUpdate(record[i]);
                int Time = (int)(record[i]->returnTime - record[i]->borrowTime);
                int Day = Time / (60 * 60 * 24);
                int BD = MAX_BORROW_TIME;//借书期限
                if (record[i]->isRenewed == true)
                    BD = 2 * MAX_BORROW_TIME;
                if (Day > BD)//0超期还书
                    return 0;
                else //1正常还书
                    return 1;
            }
        }
        return -1;
    }


    //最后一个属性是要增加书的数量
    int AddBook(string Isbn, string Name, string Author, string Publisher, double price,int nums)
    {
        Book **books = SelectBookIsbn(Isbn);
        if(books[0]==nullptr){
            Book *book = Create<Book>();
            strcpy(book->Name,&Name[0]);
            strcpy(book->ISBN, &Isbn[0]);
            strcpy(book->bookAuthor, &Author[0]);
            strcpy(book->bookPublish, &Publisher[0]);
            book->canBorrow = false;
            book->price = price;
            book->isDeleted = false;
            InsertOrUpdate(book);
            for(int n =nums-1;n>0;n--){
                book = Create<Book>();
                strcpy(book->Name,&Name[0]);
                strcpy(book->ISBN, &Isbn[0]);
                strcpy(book->bookAuthor, &Author[0]);
                strcpy(book->bookPublish, &Publisher[0]);
                book->canBorrow = true;
                book->price = price;
                book->isDeleted = false;
                InsertOrUpdate(book);
            }
            return 1;
        }else{
            Book *book = nullptr;
            if(strcmp(Name.c_str(),books[0]->Name)||strcmp(Author.c_str(),books[0]->bookAuthor)||strcmp(Publisher.c_str(),books[0]->bookPublish)||price!=books[0]->price){
                return -1;
            }
            else{
                for(int n = nums;n>0;n--){
                    book = Create<Book>();
                    strcpy(book->Name,&Name[0]);
                    strcpy(book->ISBN, &Isbn[0]);
                    strcpy(book->bookAuthor, &Author[0]);
                    strcpy(book->bookPublish, &Publisher[0]);
                    book->canBorrow = true;
                    book->price = price;
                    book->isDeleted = false;
                    InsertOrUpdate(book);
                }
                return 1;
            }
        }
    }

    //删除图书 成功1 书不存在-1 书正被借出0
    int DeleteBook(int ID)
    {
        auto book = SelectID<Book>(ID);
        if (book->ID == -1||book->isDeleted) {
            return -1;
        }
        else if(!book->canBorrow){
            return 0;
        }
        else {
            Delete<Book>(book->ID);
            return 1;
        }
    }
}
