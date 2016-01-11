#ifndef CONFIGURE_H
#define CONFIGURE_H

namespace Configure{

    #define BOOK_ISBN_LEN 20
    #define BOOK_NAME_LEN 100
    #define BOOK_AUTHOR_LEN 50
    #define BOOK_PUBLISHER_LEN 50
    #define USER_NAME_LEN 50
    #define USER_PASSWORD_LEN 40
    #define USER_TYPE_LEN 10
    #define MAX_BORROW_TIME 30  //最大借书天数
    #define MaxBuffer 10000     //缓存数据数目

    //数据存储文件所在的目录
    extern const char* DATA_PATH;
}


#endif // CONFIGURE_H
