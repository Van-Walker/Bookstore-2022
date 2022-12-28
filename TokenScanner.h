#ifndef BOOKSTORE_2022_TOKENSCANNER_H
#define BOOKSTORE_2022_TOKENSCANNER_H

#include "Book.h"

void Work(char ch[], bool &flag);

bool IsNumber(char a);

bool IsLetter(char a);

void CheckLen(const string &str, const int &len);

void CheckPrint(const string &str);

void CheckNumber(const string &str);

void Check(const string &str);

void CheckFloat(const string &str);

void CheckChar(char a);

void CheckKeyword(const string &str, bool flag);

DataType GetType(const string &str);


bool IsNumber(char a) { return (a >= '0' && a <= '9'); }

bool IsLetter(char a) { return ((a >= 'a' && a <= 'z') || (a >= 'A' && a <= 'Z')); }

void CheckLen(const string &str, const int &len) {
    if (str.size() > len) { throw error(); }
}

void CheckPrint(const string &str) {
    for (int i = 0; i < str.size(); ++i) {
        if (!isprint(str[i])) { throw error(); }
    }
}

void CheckNumber(const string &str) {
    int len = str.size();
    if (len > 10) { throw error(); }
    for (int i = 0; i < len; ++i) {
        if (!IsNumber(str[i])) { throw error(); }
    }
    if (stoll(str) > 2147483647) { throw error(); }
}

void Check(const string &str) {
    int len = str.size();
    if (len > 30) { throw error(); }
    for (int i = 0; i < len; ++i) {
        if ((!IsNumber(str[i])) && (!IsLetter(str[i])) && str[i] != '_') { throw error(); }
    }
}

void CheckFloat(const string &str) {
    int len = str.size(), cnt = 0;
    if (len > 13) { throw error(); }
    if (str[0] == '.' || str[len - 1] == '.') { throw error(); }
    if (str[0] == '0' && (len > 1 && str[1] != '.')) { throw error(); }
    for (int i = 0; i < len; ++i) {
        if ((!IsNumber(str[i])) && str[i] != '.') { throw error(); }
        if (str[i] == '.') { ++cnt; }
    }
    if (cnt > 1) { throw error(); }
}

void CheckChar(char a) {
    if (a <= 31 || a >= 127) { throw error(); }
}

void CheckKeyword(const string &str, bool flag) {
    if (str.size() > 60) { throw error(); }
    if (flag && (str[0] == '|' || str[str.size() - 1] == '|')) { throw error(); }
    for (int i = 0; i < str.size(); ++i) {
        if (str[i] == '\"') { throw error(); }
    }
}

DataType GetType(const string &str) {
    int len = str.size();
    if (len >= 6 && str.substr(0, 6) == "-ISBN=") { return ISBN; }
    if (len >= 7 && str.substr(0, 7) == "-name=\"") { return Name; }
    if (len >= 9 && str.substr(0, 9) == "-author=\"") { return Author; }
    if (len >= 10 && str.substr(0, 10) == "-keyword=\"") { return Keyword; }
    if (len >= 7 && str.substr(0, 7) == "-price=") { return Price; }
    throw error();
}

void Work(char ch[], bool &flag) {
    try {
        string str[20];
        Account user_now = acc_sys.GetUser();
        for (int i = 0; i < 20; ++i) { str[i] = ""; }
        int cnt = 0;
        int start = 0, len = strlen(ch);
        while (ch[start] == ' ') { ++start; }
        for (int i = start; i < len; ++i) {
            CheckChar(ch[i]);
            if (cnt >= 20) { throw error(); }
            if (ch[i] == ' ') {
                if (i < len - 1 && ch[i + 1] != ' ') { ++cnt; }
                continue;
            }
            str[cnt] += ch[i];
        }
        // 拆分指令
        if (!str[0].size()) { return; }
        if (str[0] == "quit" || str[0] == "exit") {
            if (cnt > 0) { throw error(); }
            flag = false;
            return;
        }
        if (str[0] == "su") {
            if (cnt == 0) { throw error(); }
            Check(str[1]), CheckLen(str[1], 30);
            if (cnt == 1) { acc_sys.Login(myString(str[1])); }
            else if (cnt == 2) {
                Check(str[2]), CheckLen(str[2], 30);
                acc_sys.Login(myString(str[1]), myString(str[2]));
            } else { throw error(); }
        } else if (str[0] == "logout") {
            if (cnt != 0) { throw error(); }
            acc_sys.Logout();
        } else if (str[0] == "register") {
            if (cnt != 3) { throw error(); }
            Check(str[1]), CheckLen(str[1], 30);
            Check(str[2]), CheckLen(str[2], 30);
            CheckPrint(str[3]), CheckLen(str[3], 30);
            acc_sys.Register(myString(str[1]), myString(str[2]), myString(str[3]));
        } else if (str[0] == "passwd") {
            if (cnt < 2) { throw error(); }
            Check(str[1]), CheckLen(str[1], 30);
            Check(str[2]), CheckLen(str[2], 30);
            if (cnt == 2) { acc_sys.EditPassword(myString(str[1]), myString(str[2])); }
            else if (cnt == 3) {
                Check(str[3]), CheckLen(str[3], 30);
                acc_sys.EditPassword(myString(str[1]), myString(str[2]), myString(str[3]));
            } else { throw error(); }
        } else if (str[0] == "useradd") {
            if (cnt != 4) { throw error(); }
            Check(str[1]), CheckLen(str[1], 30);
            Check(str[2]), CheckLen(str[2], 30);
            CheckNumber(str[3]), CheckLen(str[3], 1);
            CheckPrint(str[4]), CheckLen(str[4], 30);
            acc_sys.UserAdd(myString(str[1]), myString(str[2]), myString(str[4]), str[3][0] - '0');
        } else if (str[0] == "delete") {
            if (cnt != 1) { throw error(); }
            Check(str[1]), CheckLen(str[1], 30);
            acc_sys.Delete(myString(str[1]));
        } else if (str[0] == "buy") {
            if (cnt != 2) { throw error(); }
            CheckPrint(str[1]), CheckLen(str[1], 20);
            CheckNumber(str[2]);
            book_sys.Buy(myString(str[1]), myString(str[2]));
        } else if (str[0] == "select") {
            if (cnt != 1) { throw error(); }
            CheckPrint(str[1]), CheckLen(str[1], 20);
            book_sys.Select(myString(str[1]));
        } else if (str[0] == "modify") {
            int type = 0;
            string isbn, name, author, keyword, price;
            for (int i = 1; i <= cnt; ++i) {
                DataType now = GetType(str[i]);
                if (type & (1 << now)) { throw error(); }
                type |= (1 << now);
                if (now == ISBN) {
                    CheckLen(str[i], 26);
                    isbn = str[i].substr(6);
                    if (isbn.size() == 0) { throw error(); }
                } else if (now == Name) {
                    if (str[i][str[i].size()-1] != '\"') { throw error(); }
                    CheckKeyword(str[i].substr(7, str[i].size() - 8), false);
                    name = str[i].substr(7, str[i].size() - 8);
                    if (name.size() == 0) { throw error(); }
                } else if (now == Author) {
                    if (str[i][str[i].size()-1] != '\"') { throw error(); }
                    CheckKeyword(str[i].substr(9, str[i].size() - 10), false);
                    author = str[i].substr(9, str[i].size() - 10);
                    if (author.size() == 0) { throw error(); }
                } else if (now == Keyword) {
                    if (str[i][str[i].size()-1] != '\"') { throw error(); }
                    CheckKeyword(str[i].substr(10, str[i].size() - 11), true);
                    keyword = str[i].substr(10, str[i].size() - 11);
                    if (keyword.size() == 0) { throw error(); }
                } else if (now == Price) {
                    if (str[i].size() <= 7) { throw error(); }
                    CheckFloat(str[i].substr(7));
                    price = str[i].substr(7);
                }
            }
            if (type == 0) { throw error(); }
            book_sys.Modify(type, myString(isbn), myString(name), myString(author), myString(keyword), myString(price));
        } else if (str[0] == "import") {
            if (cnt != 2) { throw error(); }
            CheckNumber(str[1]);
            CheckFloat(str[2]);
            book_sys.Import(myString(str[1]), myString(str[2]));
        } else if (str[0] == "show") {
            if (cnt >= 1 && str[1] == "finance") {
                if (cnt == 1) { log_sys.Show(); }
                else if (cnt == 2) {
                    CheckNumber(str[2]);
                    log_sys.Show(str[2]);
                } else { throw error(); }
            } else {
                if (cnt == 0) { book_sys.Show("", Any); }
                else {
                    if (cnt > 1) { throw error(); }
                    DataType type = GetType(str[1]);
                    string inf;
                    if (type == ISBN) {
                        CheckLen(str[1], 26);
                        inf = str[1].substr(6);
                        CheckPrint(inf);
                    } else if (type == Name) {
                        if (str[1][str[1].size() - 1] != '\"') { throw error(); }
                        CheckKeyword(str[1].substr(7, str[1].size() - 8), false);
                        inf = str[1].substr(7, str[1].size() - 8);
                    } else if (type == Author) {
                        if (str[1][str[1].size()-1] != '\"') { throw error(); }
                        CheckKeyword(str[1].substr(9, str[1].size() - 10), false);
                        inf = str[1].substr(9, str[1].size() - 10);
                    } else if (type == Keyword) {
                        if (str[1][str[1].size()-1] != '\"') { throw error(); }
                        CheckKeyword(str[1].substr(10, str[1].size() - 11), true);
                        inf = str[1].substr(10, str[1].size() - 11);
                        for (int i = 0; i < inf.size(); ++i) {
                            if (inf[i] == '|') { throw error(); }
                        }
                    } else { throw error(); }
                    book_sys.Show(myString(inf), type);
                }
            }
        } else if (str[0] == "log") {
            if (cnt != 0) { throw error(); }
            log_all.Log();
        } else { throw error(); }
        if (str[0] != "log") {
            log_all.AddLog(user_now, ch);
        }
    }
    catch (error) { std::cout << "Invalid\n"; }
}

#endif //BOOKSTORE_2022_TOKENSCANNER_H