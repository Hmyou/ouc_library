#pragma once
#include <configure.h>
#include <cstdio>
#include <cstdlib>
#include <list>
#include <string.h>
#include <iostream>
#include <functional>
#include <qstring.h>
#include <direct.h>
#include <qregexp.h>

using namespace std;

namespace DataManagement{    
    // 图书信息
    struct Book
    {
        int  ID;//自定义编号
        char Name[BOOK_NAME_LEN];//书名
        char bookAuthor[BOOK_AUTHOR_LEN];//作者
        char bookPublish[BOOK_PUBLISHER_LEN];//出版商
        char ISBN[BOOK_ISBN_LEN];//isbn编码
        double  price;//价格
        bool canBorrow;//是否可借
        bool isDeleted;//是否存在
    };

	// 用户
	struct User
	{
		int  ID;   
		char Name[USER_NAME_LEN];//用户名
		char password[USER_PASSWORD_LEN];//密码
		char type[USER_TYPE_LEN];//用户类型
		unsigned int borrowMax;//可接书的最大数目
		bool isDeleted;//是否存在
    };

    // 借阅记录 理论上记录不会被删除
    struct Record
    {
        int ID;
        int userId;//用户id
        int bookId;//书id
        time_t borrowTime; //借阅时间
        time_t returnTime; //返还时间
        bool isBorrowed;//书是否归还
        bool isRenewed;//是否续借
    };


	// 产生文件路径
    template <typename T>
    char* GenerateFilePath();



	//类型文件是否存在
	template <typename T>
	bool FileIsExist_internal() {
		FILE *file;
		auto exist = true;
		if (fopen_s(&file, GenerateFilePath<T>(), "rb") != 0) {
			exist = false;
		}
		else {
			fclose(file);
		}
		return exist;
	}

	// 打开存储某种类型的文件
	template <typename T>
	FILE* OpenFile() {
		FILE *file = fopen(GenerateFilePath<T>(), "rb+");
		auto i = MaxBuffer;
		if (file == NULL) {
            return NULL;
		}
		else//允许程序控制缓冲和stream 的缓冲区大小
            setvbuf(file, NULL, _IOFBF, (sizeof (T))*i);
		return file;
	}

	// 根据 ID 选择某个记录
    // -1无此书
	template <typename T>
    T*
    SelectID(int ID) {
		auto file = OpenFile<T>();
        auto size = sizeof (T);
		auto item = new T();
        item->ID = -1;
        if(ID>=0){
            fseek(file, size * ID, SEEK_SET);
            fread(item, size, 1, file);
        }
		return item;
	}


    //book的isbn查询
    Book** SelectBookIsbn_internal(string Isbn);



	//用户的借阅记录,userid查询
    Record** SelectRecordUserID_internal(int userID);

	//用户的借阅记录,双id查询
    Record** SelectRecordTwoID_internal(int userID, int bookID);

	//名字相等
	template <typename T, typename T1>
    bool NameEqual(T*item, T1 condition) {
        return !strcmp(item->Name, condition.c_str());
    }

    //限制选择的最大数目
    template <typename T, typename T1>
    bool IsMaxCount(T1 condition, T* item, list<T*>&petitList, int maxCount) {
        int count = 0;
        if (NameEqual(item,condition)) {
                T* t = new T();
                memcpy(t, item, sizeof (T));
                petitList.push_back(t);
                count++;
        }
        return count != maxCount; //返回值是 数据不达到最大值->真
    }



	// 名字搜索,返回全部;
	template <typename T, typename T1>
	T** SelectNameTous(T1 condition, int beginID = 0, int maxCount = -1) {
		auto temp = list<T*>();
		TravelTous(condition, temp, maxCount);
		auto result = new T*[temp.size() + 1];
		int i = 0;
		for (auto it = temp.begin(); it != temp.end(); it++) {
			result[i] = *it;
			i++;
		}
		result[temp.size()] = NULL;
		return result;
	}


	// 名字搜索,返回符合条件的第1个记录，没有找到时返回 NULL
	template <typename T, typename T1>
	T* SelectNameOne(T1 condition, int beginID = 0) {
		auto results = SelectNameTous<T>(condition, beginID, 1);
		auto result = *results;
		delete[] results;
		return result;
	}

    // 遍历一种记录
    template <typename T, typename T1>
    void TravelTous(T1 condition,list<T*>&petitList, int maxCount, int beginID = 0, int endID = 0) {
        auto file = OpenFile<T>();
        auto size = sizeof (T);
        T* item = new T();
        fseek(file, 0, SEEK_END);
        auto count = ftell(file) / size;
        if (endID <= 0) {
            endID += count;
        }
        fseek(file, beginID * size, SEEK_SET);
        for (auto i = beginID; i < endID; i++) {
            fread(item, size, 1, file);
            if (!item->isDeleted && !IsMaxCount(condition, item, petitList,maxCount)) {
                break;
            }
        }
        delete item;
    }

	// 创建空记录
	template <typename T>
	T* Create() {
		auto item = new T();
        memset(item, 0, sizeof (T));
		item->ID = -1;
		return item;
	}

	// 插入或更新一个记录,返回id
	template <typename T>
	int InsertOrUpdate(T* item) {
		auto file = OpenFile<T>();
        auto size = sizeof (T);
		if (item->ID == -1) {
			fseek(file, 0, SEEK_END);
			auto offset = ftell(file);
			item->ID = offset / size;
		}
		else {
			fseek(file, size * item->ID, SEEK_SET);
		}
		fwrite(item, size, 1, file);
		fclose(file);
		return item->ID;
	}

	// 删除一个记录
	template <typename T>
	bool Delete(int ID) {
		auto file = OpenFile<T>();
        auto size = sizeof (T);
		T* item = new T();

		fseek(file, size * ID, SEEK_SET);
		fread(item, size, 1, file);
		item->isDeleted = true;
		fseek(file, size * ID, SEEK_SET);
		auto i = 0;
		i = fwrite(item, size, 1, file);
		delete item;
		fclose(file);
		if (i == 1)
			return true;
		else
			return false;
	}

    //book的记录查询
    Record** SelectBookRecord_internal(int ID );

    void DataInit(bool forceInit);

}
//以下函数功能注释在cpp中查看
QString s_q(const string &s);
string q_s(const QString &s);
bool isAllNumber(const char *str);
bool numCheck(string str,int num);
bool isbnCheck(string str);
int _isspace(int ch);
string &_ltrim(string &s);
string &_rtrim(string &s);
string &_trim(string &s);


