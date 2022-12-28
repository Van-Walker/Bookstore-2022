#ifndef BOOKSTORE_2022_BOOK_H
#define BOOKSTORE_2022_BOOK_H

#include <iomanip>
#include <algorithm>
#include "UnrolledLinkedList.h"
#include "Account.h"
#include "Log.h"

struct Book {
    myString ISBN;
    myString Name;
    myString Author;
    myString Keyword;
    int Quantity;
    double Price, TotalCost;

    Book(myString isbn = "", myString name = "", myString author = "", myString keyword = "", int quantity = 0, double price = 0, double totalcost = 0) {
        ISBN = isbn, Name = name, Author = author, Keyword = keyword, Quantity = quantity, Price = price, TotalCost = totalcost;
    }

    bool operator <(const Book &rhs) const { return ISBN < rhs.ISBN; }

    friend std::ostream &operator <<(std::ostream &os, const Book &now);
};

class BookSystem {
private:
    UnrolledLinkedList <myString, int> ISBN_index;
    UnrolledLinkedList <myString, int> Name_index;
    UnrolledLinkedList <myString, int> Author_index;
    UnrolledLinkedList <myString, int> Keyword_index;
    std::fstream file;
    const std::string file_name = "Book";
    int book_cnt;

public:
    BookSystem();

    ~BookSystem();

    int ISBN_to_pos(myString ISBN);

    void Show(myString inf, DataType type);

    void Buy(myString ISBN, myString Quantity);

    void Insert(Book &now, int pos);

    void Delete(Book &now, int pos);

    void Select(myString ISBN);

    void Modify(int type, myString ISBN, myString Name, myString Author, myString Keyword, myString Price);

    void Import(myString Quantity, myString TotalCost);
};

std::ostream &operator <<(std::ostream &os, const Book &now) {
    os << now.ISBN << '\t' << now.Name << '\t' << now.Author << '\t' << now.Keyword << '\t';
    os << std::fixed << std::setprecision(2) << now.Price << '\t' << now.Quantity << '\n';
    return os;
}

BookSystem::BookSystem() : ISBN_index("ISBN"), Name_index("Name"), Author_index("Author"), Keyword_index("Keyword") {
    file.open(file_name);
    if (!file.good()) {
        file.open(file_name, std::fstream::out);
        file.close();
        file.open(file_name);
        book_cnt = -1;
    } else {
        file.seekg(0);
        file.read(reinterpret_cast <char *> (&book_cnt), sizeof(book_cnt));
    }
}

BookSystem::~BookSystem() {
    file.seekp(0);
    file.write(reinterpret_cast <char *> (&book_cnt), sizeof(book_cnt));
    file.close();
}

int BookSystem::ISBN_to_pos(myString ISBN) {
    std::vector <int> ret = ISBN_index.Find(ISBN);
    if (ret.empty()) { return -1; }
    return ret[0];
}

void BookSystem::Show(myString inf, DataType type) {
    Account now_user = acc_sys.GetUser();
    if (now_user.Privilege < 1) { throw error(); }
    vector <int> ret;
    ret.clear();
    if (type == ISBN) {
        ret = ISBN_index.Find(inf);
    } else if (type == Name) {
        ret = Name_index.Find(inf);
    } else if (type == Author) {
        ret = Author_index.Find(inf);
    } else if (type == Keyword) {
        ret = Keyword_index.Find(inf);
    } else if (type == Any) {
        ret = ISBN_index.FindAll();
    }
    if (ret.empty()) {
        cout << '\n';
        return;
    }
    vector <Book> res;
    res.clear();
    for (int i = 0; i < ret.size(); ++i) {
        int pos = ret[i];
        Book now;
        file.seekg(pos * sizeof(now) + sizeof(int));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        res.push_back(now);
    }
    std::sort(res.begin(), res.end());
    for (int i = 0; i < res.size(); ++i) {
        cout  <<  res[i];
    }
}

void BookSystem::Buy(myString ISBN, myString Quantity) {
    Account now_user = acc_sys.GetUser();
    if (now_user.Privilege < 1) { throw error(); }
    int pos = ISBN_to_pos(ISBN);
    if (pos == -1) { throw error(); }
    Book now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    int quantity = Quantity.to_int();
    if (quantity > now.Quantity) { throw error(); }
    if (quantity == 0) { throw error(); }
    now.Quantity -= quantity;
    file.seekp(pos * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
    cout << std::fixed << std::setprecision(2) << quantity * now.Price << "\n";
    log_sys.RecordLog(now.Price * quantity, 0);
}

void BookSystem::Insert(Book &now, int pos) {
    ISBN_index.Insert(node <myString, int> (now.ISBN, pos));
    Name_index.Insert(node <myString, int> (now.Name, pos));
    Author_index.Insert(node <myString, int> (now.Author, pos));
    string s = "";
    for (int i = 0; i < now.Keyword.size(); ++i) {
        if (now.Keyword[i] == '|') {
            Keyword_index.Insert(node <myString, int> (s, pos));
            s = "";
        } else { s += now.Keyword[i]; }
    }
    if (s.size()) { Keyword_index.Insert(node <myString, int> (s, pos)); }
}

void BookSystem::Delete(Book &now, int pos) {
    ISBN_index.Delete(node <myString, int> (now.ISBN, pos));
    Name_index.Delete(node <myString, int> (now.Name, pos));
    Author_index.Delete(node <myString, int> (now.Author, pos));
    string s = "";
    for (int i = 0; i < now.Keyword.size(); ++i) {
        if (now.Keyword[i] == '|') {
            Keyword_index.Delete(node <myString, int> (s, pos));
            s = "";
        } else { s += now.Keyword[i]; }
    }
    if (s.size()) { Keyword_index.Delete(node <myString, int> (s, pos)); }
}

void BookSystem::Select(myString ISBN) {
    Account now_user = acc_sys.GetUser();
    if (now_user.Privilege < 3) { throw error(); }
    int pos = ISBN_to_pos(ISBN);
    if (pos == -1) {
        Book now(ISBN);
        ++book_cnt;
        Insert(now, book_cnt);
        file.seekp(book_cnt * sizeof(now) + sizeof(int));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
        acc_sys.users.pop();
        acc_sys.users.push(make_pair(now_user, book_cnt));
        // 之前忘记把新的选择情况推进stack里了
    } else {
        acc_sys.users.pop();
        acc_sys.users.push(make_pair(now_user, pos));
    }
}

void BookSystem::Modify(int type, myString ISBN, myString Name, myString Author, myString Keyword, myString Price) {
    Account now_user = acc_sys.GetUser();
    if (now_user.Privilege < 3) { throw error(); }
    int pos = acc_sys.users.top().second;
    if (pos == -1) { throw error(); }
    Book now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));

    Book to = now;
    if (type & (1 << 4)) {
        if (now.ISBN == ISBN) { throw error(); }
        int pos_new = ISBN_to_pos(ISBN);
        if (pos_new != -1) { throw error(); }
        to.ISBN = ISBN;
    }
    if (type & (1 << 3)) { to.Name = Name; }
    if (type & (1 << 2)) { to.Author = Author; }
    if (type & (1 << 1)) {
        to.Keyword = Keyword;
        int cnt = 0;
        string s[10];
        for (int i = 0; i < Keyword.size(); ++i) {
            if (Keyword[i] == '|') { ++cnt; }
            else { s[cnt] += Keyword[i]; }
        }
        for (int i = 0; i < cnt; ++i) {
            if (!s[i].size()) { throw error(); }
            for (int j = i + 1; j <= cnt; ++j) {
                if (s[i] == s[j]) { throw error(); }
            }
        }
    }
    if (type & 1) { to.Price = Price.to_double(); }
    Delete(now, pos);
    Insert(to, pos);
    file.seekp(pos * sizeof(to) + sizeof(int));
    file.write(reinterpret_cast <char *> (&to), sizeof(to));
}

void BookSystem::Import(myString Quantity, myString TotalCost) {
    Account now_user = acc_sys.GetUser();
    if (now_user.Privilege < 3) { throw error(); }
    int pos = acc_sys.users.top().second;
    if (pos == -1) { throw error(); }
    int quantity = Quantity.to_int();
    if (quantity == 0) { throw error(); }
    double totalcost = TotalCost.to_double();
    if (totalcost == 0) { throw error(); }
    Book now;
    file.seekg(pos * sizeof(now) + sizeof(int));
    file.read(reinterpret_cast <char *> (&now), sizeof(now));
    now.Quantity += quantity, now.TotalCost += totalcost;
    file.seekp(pos * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
    log_sys.RecordLog(0, totalcost);
}

BookSystem book_sys;

#endif //BOOKSTORE-2022_BOOK_H