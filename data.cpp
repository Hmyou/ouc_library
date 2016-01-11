#include "configure.h"
#include "data.h"
using namespace Configure;



namespace DataManagement{
    template <typename T>
    char* GenerateFilePath(){
        auto name = typeid(T).name();
        auto lengthName = strlen(name);
        auto lengthPath = strlen(DATA_PATH);
        auto filepath = new char[lengthPath + lengthName + 5];
        strcpy(filepath,DATA_PATH);
        strcpy(filepath + lengthPath,name);
        for (auto i = lengthPath; i < lengthPath + lengthName; i++) {
            if (strchr("\\/:*?\"<>| ", filepath[i]) != NULL) {
                filepath[i] = '_';
            }
        }
        filepath[lengthPath + lengthName] = '.';
        filepath[lengthPath + lengthName + 1] = 'd';
        filepath[lengthPath + lengthName + 2] = 'a';
        filepath[lengthPath + lengthName + 3] = 't';
        filepath[lengthPath + lengthName + 4] = '\0';
        return filepath;
    }


    void DataInit(bool forceInit = false) {
        _mkdir(DATA_PATH);
        if (forceInit || !FileIsExist_internal<Book>()) {
            fclose(fopen(GenerateFilePath<Book>(), "wb"));
        }
        if (forceInit || !FileIsExist_internal<User>()) {
            fclose(fopen(GenerateFilePath<User>(), "wb"));
            User* user;
            user = Create<User>();
            strcpy(user->Name, "root");
            strcpy(user->password, "root");
            strcpy(user->type, "admin");
            InsertOrUpdate(user);
            delete user;

        }
        if (forceInit || !FileIsExist_internal<Record>()) {
            fclose(fopen(GenerateFilePath<Record>(), "wb"));
        }
    }


    //book的记录查询
    Record** SelectBookRecord_internal(int ID ) {
        auto temp = list<Record*>();
        auto file = OpenFile<Record>();
        auto size = sizeof Record;
        Record* item = new Record();
        fseek(file, 0, SEEK_END);
        int endID = ftell(file) / size;
        for (int i = 0; i < endID; i++) {
            fseek(file, i *size, SEEK_SET);
            fread(item, size, 1, file);
            if(item->bookId == ID) {
                Record* t = new Record();
                memcpy(t, item, sizeof Record);
                temp.push_back(t);
            }
        }
        delete item;
        auto result = new Record*[temp.size() + 1];
        int j = 0;
        for (auto it = temp.begin(); it != temp.end(); it++) {
            result[j] = *it;
            j++;
        }
        result[temp.size()] = NULL;
        fclose(file);
        return result;
    }


    //book的isbn查询
    Book** SelectBookIsbn_internal(string Isbn) {
        auto temp = list<Book*>();
        auto file = OpenFile<Book>();
        auto size = sizeof Book;
        Book* item = new Book();
        fseek(file, 0, SEEK_END);
        int endID = ftell(file) / size;
        for (int i = 0; i < endID; i++) {
            fseek(file, i *size, SEEK_SET);
            fread(item, size, 1, file);
            if(0==strcmp(item->ISBN, Isbn.c_str())) {
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
        fclose(file);
        return result;
    }



    //用户的借阅记录,userid查询
    Record** SelectRecordUserID_internal(int userID) {
        auto temp = list<Record*>();
        auto file = OpenFile<Record>();
        auto size = sizeof Record;
        Record* item = new Record();
        fseek(file, 0, SEEK_END);
        int endID = ftell(file) / size;
        for (int i = 0; i < endID; i++) {
            fseek(file, i *size, SEEK_SET);
            fread(item, size, 1, file);
            if (item->userId == userID) {
                Record* t = new Record();
                memcpy(t, item, sizeof Record);
                temp.push_back(t);
            }
        }
        delete item;
        auto result = new Record*[temp.size() + 1];
        int j = 0;
        for (auto it = temp.begin(); it != temp.end(); it++) {
            result[j] = *it;
            j++;
        }
        result[temp.size()] = NULL;
        return result;
    }

    //用户的借阅记录,双id查询
    Record** SelectRecordTwoID_internal(int userID, int bookID) {
        auto temp = list<Record*>();
        auto file = OpenFile<Record>();
        auto size = sizeof Record;
        Record* item = new Record();
        fseek(file, 0, SEEK_END);
        int endID = ftell(file) / size;
        for (int i = 0; i < endID; i++) {
            fseek(file, i *size, SEEK_SET);
            fread(item, size, 1, file);
            if (item->userId == userID && item->bookId == bookID) {
                Record* t = new Record();
                memcpy(t, item, sizeof Record);
                temp.push_back(t);
            }
        }
        delete item;
        auto result = new Record*[temp.size() + 1];
        int j = 0;
        for (auto it = temp.begin(); it != temp.end(); it++) {
            result[j] = *it;
            j++;
        }
        result[temp.size()] = NULL;
        return result;
    }

}

//string类型转换成QString类型
QString s_q(const string &s)
{
    return QString(QString::fromLocal8Bit(s.c_str()));
}
//QString类型转换成string类型
string q_s(const QString &s)
{
    return string((const char *)s.toLocal8Bit());
}
//判断char[]是否全为数字类型
bool isAllNumber(const char *str){
    if('0' == str[0]){
        if('\0' == str[1]){
            return true;
        }
        return false;
    }
    int i = 0;
    while(str[i]!='\0'){
        if(str[i] < '0' || str[i] >'9'){
            return false;
        }
        i++;
    }
    return true;
}
//判断str是否为num位数字
bool numCheck(string str, int num){
    if((str.size() != num)){
        return false;
    }
    if(str.size()==num){
        if(isAllNumber(str.c_str())){
            return true;
        }
        else return false;
    }else{
        return false;
    }
    return false;
}

//ISBN检测
bool isbnCheck(string str){
    if(numCheck(str,10)||numCheck(str,13)){
        return true;
    }
    else return false;
}

// 判断字符ch是否为空白符。空白符指空格、水平制表、垂直制表、换页、回车和换行符
int _isspace(int ch){
    return (unsigned int)(ch - 9) < 5u  ||  ch == ' ';
}
//去掉首尾空格
// trim from start
string &_ltrim(std::string &s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(_isspace))));
        return s;
}

// trim from end
string &_rtrim(string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(_isspace))).base(), s.end());
        return s;
}

// trim from both ends
string &_trim(string &s) {
        return _ltrim(_rtrim(s));
}
