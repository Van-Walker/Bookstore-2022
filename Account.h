#ifndef BOOKSTORE_2022_CPP_ACCOUNT_H
#define BOOKSTORE_2022_CPP_ACCOUNT_H

#include "UnrolledLinkedList.h"
#include <stack>

struct Account {
    myString ID;
    myString Password, Name;
    int Privilege;
    bool in_stack;
    Account();
    Account(myString id, myString password, myString name, int privilege) {
        ID = id, Password = password, Name = name, Privilege = privilege, in_stack = false;
    }

};

class AccountSystem {
private:
    UnrolledLinkedList <myString, int> ID_index;
    std::fstream file;
    const std::string file_name = "Account";
    int account_cnt = -1;
    std::stack <Account> users;

public:
    AccountSystem() : ID_index("ID") {
        file.open(file_name);
        if (!file.good()) {
            file.open(file_name, std::fstream::out);
            file.close();
            file.open(file_name);
        }
        while (users.size()) { users.pop(); }
    }

    ~AccountSystem() {
        file.close();
        while (users.size()) { users.pop(); }
    }

    int ID_to_pos(myString &ID) {
        std::vector <int> ret = ID_index.Find(ID);
        if (ret.empty()) { return -1; }
        return ret[0];
    }

    void UserAdd(myString &ID, myString &Password, myString &Name, int &Privilege) {
        // todo: 指令权限 3
        if (ID_to_pos(ID) != -1) {
            // todo: 重名，操作失败
        }
        // todo: 如果创建用户权限大于当前，操作失败
        Account now(ID, Password, Name, Privilege);
        ++account_cnt;
        file.seekp(account_cnt * sizeof(now));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
        ID_index.Insert(node <myString, int> (ID, account_cnt));
    }

    void Delete(myString &ID) {
        // todo: 指令权限 7
        int pos = ID_to_pos(ID);
        if (pos == -1) {
            // todo: 不存在，删除失败
        }
        // todo: 已登录，删除失败
        ID_index.Delete(node <myString, int> (ID, pos));
    }

    void Login(myString &ID, myString &Password) {
        // todo: 指令权限 0
        int pos = ID_to_pos(ID);
        if (pos == -1) {
            // todo: 不存在，登录失败
        }
        Account now;
        file.seekp(pos * sizeof(now));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        if (now.Password != Password) {
            // todo: 密码错误，登录失败
        }
    }

    void Login(myString &ID) {
        int pos = ID_to_pos(ID);
        if (pos == -1) {
            // todo: 不存在，登录失败
        }
        // todo: 若当前账户高于登录账户，可省略密码
    }

    void Logout() {
        if (!users.empty()) { users.pop(); }
        else {
            // todo: 退出登录当前账户，若栈为空则失败
        }
    }

    void Register(myString &ID, myString &Password, myString &Name) {
        // todo: 指令权限 0
        int pos = ID_to_pos(ID);
        if (pos != -1) {
            // todo: 重名，操作失败
        }
        Account now(ID, Password, Name, 1);
        ++account_cnt;
        file.seekp(account_cnt * sizeof(now));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
        ID_index.Insert(node <myString, int> (ID, account_cnt));
    }

    void EditPassword(myString &ID, myString &cur_pw, myString &new_pw) {
        // todo: 指令权限 1
        int pos = ID_to_pos(ID);
        if (pos == -1) {
            // todo: 不存在，操作失败
        }
        Account now;
        file.seekg(pos * sizeof(now));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        if (now.Password != cur_pw) {
            // todo: 密码错误，操作失败
        }
        now.Password = new_pw;
        file.seekp(pos * sizeof(now));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
    }

    void EditPassword(myString &ID, myString &new_pw) {
        // todo: 指令权限 7
        int pos = ID_to_pos(ID);
        if (pos == -1) {
            // todo: 不存在，操作失败
        }
        Account now;
        file.seekg(pos * sizeof(now));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        now.Password = new_pw;
        file.seekp(pos * sizeof(now));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
    }
};

#endif //BOOKSTORE_2022_CPP_ACCOUNT_H
