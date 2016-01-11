#include "usermanagement.h"

using namespace std;

namespace UserManagement{

    //保存用户信息文件是否存在
    bool UserIsExist(){
        return FileIsExist_internal<User>();
    }

    //查询用户
    User * SelectUser(string Name){
        return SelectNameOne<User>(Name);
    }

    //新建用户,返回id号 或者-2 存在相同用户名
    int Addaccount(string Name, string Password,string type){
        auto user = SelectUser(Name);
        if (user == nullptr) {
            auto user = Create<User>();
            strcpy(user->Name, &Name[0]);
            strcpy(user->password, &Password[0]);
            strcpy(user->type, &type[0]);
            user->borrowMax = 12;
            user->isDeleted = false;
            return InsertOrUpdate(user);
        }
        else
            return -2;
    }

    //是否有些书未还
        bool IsSomeBookToReturn(User *user){
            Record** record = SelectRecordUserID_internal(user->ID);
            int i;
            for(i=0;record[i]!=nullptr;i++){
                 if(record[i]->isBorrowed)
                     return true;
            }
            return false;
        }

    //删除用户,int型 0用户不存在 1成功
        //           -1 用户存在没还得书
        int DeleteUser(string Name){
            auto user = SelectNameOne<User>(Name);
            if (user == NULL)
                return 0;
            else if(IsSomeBookToReturn(user)){
                return -1;
            }
            else{
                Delete<User>(user->ID);
                delete user;
                user = NULL;//消除野指针
                return 1;
            }
        }

    //更新用户密码
    bool UpdataUserPassword(string Name, string Password){
        auto user = SelectNameOne<User>(Name);
        if (user == NULL)
            return false;

        memset(user->password, 0, sizeof(user->password));

        strcpy(user->password, &Password[0]);
        int i = -1;
        i = InsertOrUpdate(user);
        delete user;
        user = NULL;

        if (i != -1)
            return true;
        else
            return false;
    }

}
