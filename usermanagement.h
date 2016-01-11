#ifndef USERMANAGEMENT
#define USERMANAGEMENT

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include "data.h"
using namespace DataManagement;
using namespace std;

namespace UserManagement{
    //保存用户信息文件是否存在
    bool UserIsExist();

    //查询用户
    User * SelectUser(string Name);

    //新建用户,返回id号 或者-2 存在相同用户名
    int Addaccount(string Name, string Password,string type);

    //是否有些书未还
    bool IsSomeBookToReturn(User *user);

    //删除用户,int型 0用户不存在 1成功
    //           -1 用户存在没还的书
    int DeleteUser(string Name);

    //更新用户密码
    bool UpdataUserPassword(string Name, string Password);

}


#endif // USERMANAGEMENT

