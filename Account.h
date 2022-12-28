#ifndef BOOKSTORE_2022_CPP_ACCOUNT_H
#define BOOKSTORE_2022_CPP_ACCOUNT_H

#include "UnrolledLinkedList.h"

struct Account {
    myString ID;
    myString Password, Name;
    int Privilege;
    Account() = default;
    Account(myString id, myString password, myString name, int privilege) {
        ID = id, Password = password, Name = name, Privilege = privilege;
    }
    bool operator <(const Account &rhs) const {
        return ID < rhs.ID;
    }
};

class AccountSystem {
private:
    UnrolledLinkedList <myString, int> ID_index;
    std::fstream file;
    const std::string file_name = "Account";
    int account_cnt;
    std::set <Account> in_stack;

public:
    std::stack <pair <Account, int> > users; // first = user, second = select ( -1: no select )

    AccountSystem();

    ~AccountSystem();

    int ID_to_pos(myString ID);

    Account GetUser();

    void UserAdd(myString ID, myString Password, myString Name, int Privilege);

    void Delete(myString ID);

    void Login(myString ID, myString Password);

    void Login(myString ID);

    void Logout();

    void Register(myString ID, myString Password, myString Name, int Privilege = 1);

    void EditPassword(myString ID, myString cur_pw, myString new_pw);

    void EditPassword(myString ID, myString new_pw);
};

AccountSystem::AccountSystem() : ID_index("ID") {
    file.open(file_name);
    if (!file.good()) {
        file.open(file_name, std::fstream::out);
        file.close();
        file.open(file_name);
        account_cnt = -1;
    }
    else {
        file.seekg(0);
        file.read(reinterpret_cast <char *> (&account_cnt), sizeof(account_cnt));
    }
    while (users.size()) { users.pop(); }
    in_stack.clear();
}

AccountSystem::~AccountSystem() {
    file.seekp(0);
    file.write(reinterpret_cast <char *> (&account_cnt), sizeof(account_cnt));
    file.close();
    while (users.size()) { users.pop(); }
    in_stack.clear();
}

int AccountSystem::ID_to_pos(myString ID) {
    std::vector <int> ret = ID_index.Find(ID);
    if (ret.empty()) { return -1; }
    return ret[0];
}

Account AccountSystem::GetUser() {
    if (users.empty()) { return Account(); }
    return users.top().first;
}

void AccountSystem::UserAdd(myString ID, myString Password, myString Name, int Privilege) {
    Account user_now;
    user_now = GetUser();
    if (user_now.Privilege < 3) { throw error(); }
    if (user_now.Privilege <= Privilege ) { throw error(); }
    if (ID_to_pos(ID) != -1) { throw error(); }
    if (Privilege != 0 && Privilege != 1 && Privilege != 3 && Privilege != 7) { throw error(); }
    Account now(ID, Password, Name, Privilege);
    ++account_cnt;
    file.seekp(account_cnt * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
    ID_index.Insert(node <myString, int> (ID, account_cnt));
}

void AccountSystem::Delete(myString ID) {
    Account user_now;
    user_now = GetUser();
    if (user_now.Privilege < 7) { throw error(); }
    int pos = ID_to_pos(ID);
    if (pos == -1) { throw error(); }
    Account now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    if (in_stack.find(now) != in_stack.end()) { throw error(); }
    ID_index.Delete(node <myString, int> (ID, pos));
}

void AccountSystem::Login(myString ID, myString Password) {
    int pos = ID_to_pos(ID);
    if (pos == -1) { throw error(); }
    Account now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    if (now.Password != Password) { throw error(); }
    in_stack.insert(now);
    users.push(make_pair(now, -1));
}

void AccountSystem::Login(myString ID) {
    int pos = ID_to_pos(ID);
    if (pos == -1) { throw error(); }
    Account user_now;
    user_now = GetUser();
    Account now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    if (user_now.Privilege <= now.Privilege) { throw error(); }
    in_stack.insert(now);
    users.push(make_pair(now, -1));
}

void AccountSystem::Logout() {
    if (users.empty()) { throw error(); }
    Account now = users.top().first;
    in_stack.erase(now);
    users.pop();
}

void AccountSystem::Register(myString ID, myString Password, myString Name, int Privilege) {
    int pos = ID_to_pos(ID);
    if (pos != -1) { throw error(); }
    Account now(ID, Password, Name, Privilege);
    ++account_cnt;
    file.seekp(account_cnt * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
    ID_index.Insert(node <myString, int> (ID, account_cnt));
}

void AccountSystem::EditPassword(myString ID, myString cur_pw, myString new_pw) {
    Account user_now;
    user_now = GetUser();
    if (user_now.Privilege < 1) { throw error(); }
    int pos = ID_to_pos(ID);
    if (pos == -1) { throw error(); }
    Account now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    if (now.Password != cur_pw) { throw error(); }
    now.Password = new_pw;
    file.seekp(pos * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
}

void AccountSystem::EditPassword(myString ID, myString new_pw) {
    Account user_now;
    user_now = GetUser();
    if (user_now.Privilege < 7) { throw error(); }
    int pos = ID_to_pos(ID);
    if (pos == -1) { throw error(); }
    Account now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    now.Password = new_pw;
    file.seekp(pos * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
}

AccountSystem acc_sys;

#endif //BOOKSTORE_2022_CPP_ACCOUNT_H