# Bookstore
### _ACM 2022 Big Homework_
###### _Writen by Vanius / wt_
***
## _程序功能概述_
***
### 1. 命令行交互
### 2. 账户系统
* 信息：用户名、 ID、 密码、 登录状态    
* 操作：注册账户（游客）、 创建账户（员工）、 登录账户、 修改密码、 注销账户、 删除账户   
### 3. 图书系统
* 信息：ISBN号、 书名、 作者名、 关键词（可多个）、 单价（可改变）、 购买数量   
* 操作：根据 ISBN号 / 书名 / 作者名 / 关键词 检索图书  
* 记录购买图书  
* 修改图书信息  
* 图书进货：以指定交易总额购入指定数量的选中图书  
### 4. 日志系统
* 保存交易记录  
* 查询最后完成的指定笔数交易总额  
* 生成日志
### 5. 合法判断
* 判断每条输入是否合法，不合法应报错并放弃执行
***

## _主题逻辑说明_ 
***
* 在 main 函数中从控制台循环读入指令，交给TokenScanner处理
* TokenScanner针对当前读入指令，先确定类型，并识别明显不合法的指令（第一类错误抛出）
* 按照类型执行各类指令，若执行失败则进行第二类错误抛出
* 将各类操作执行完后，文件输出到相应的储存文件保存修改，并记录日志
* 登录栈在用户类中实现

***
## _各个类的接口及成员说明_
***

### 1. 账户类
````cpp
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
````

### 2. 图书类
````cpp
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
````

### 3. 日志类 
````cpp
    struct log {
        double income;
        double expenditure;
        log() = default;
        log(double x, double y) {
            income = x, expenditure = y;
        }
    };

    class LogSystem {
    private:
        int log_cnt;
        std::fstream file;
        string file_name = "log";

    public:
        LogSystem();

        ~LogSystem();

        void Show();

        void Show(myString Count);

        void RecordLog(double income, double expenditure);

    };

    struct Logs {
        Account user;
        char behaviour[1005];
        Logs() = default;
        Logs(Account user_, char *behaviour_) {
            user = user_;
            for (int i = 0; i < strlen(behaviour_); ++i) {
                behaviour[i] = behaviour_[i];
            }
            behaviour[strlen(behaviour_)] = '\0';
        }
    };


    class LogAll {
    private:
        int cnt;
        std::fstream file;
        string file_name = "LogAll";

    public:
        LogAll();

        ~LogAll();

        void AddLog(Account user, char *ch);

        void Log();
    };
````


### 4. 块状链表类
````cpp
    template <class KeyType, class ValueType>
    struct node {
        KeyType Key;
        ValueType Value;
        node() = default;

        node(const KeyType &key, const ValueType &value) { Key = key, Value = value; }

        inline bool operator ==(const node &rhs) const {
            if (Key == rhs.Key && Value == rhs.Value) { return true; }
            return false;
        }

        inline bool operator !=(const node &rhs) const { return !(*this == rhs); }

        inline bool operator <(const node &rhs) const {
            if (Key != rhs.Key) { return Key < rhs.Key; }
            return Value < rhs.Value;
        }

        inline bool operator >=(const node &rhs) const { return !(*this < rhs); }

        inline bool operator >(const node &rhs) const {
            if (Key != rhs.Key) { return Key > rhs.Key; }
            return Value > rhs.Value;
        }

        inline bool operator <=(const node &rhs) const { return !(*this > rhs); }
    };

    template <class KeyType, class ValueType>
    struct block {
        node <KeyType, ValueType> min_node, max_node;
        int cnt = 0;
        int next = -1;
        node <KeyType, ValueType> list[2 * SQRN + 5];
    };


    template <class KeyType, class ValueType>
    class UnrolledLinkedList {
    private:
        std::fstream file;
        std::string file_name;
        const int max_cnt = 2 * SQRN;
        const int min_cnt = SQRN;
        int block_cnt = -1;

    public:
        UnrolledLinkedList(std::string name);

        ~UnrolledLinkedList();

        void ReadBlock(int pos, block <KeyType, ValueType> &now);

        void ReadLittle(int pos, block <KeyType, ValueType> &now);

        void WriteBlock(int pos, block <KeyType, ValueType> &now);

        void split(int pos, block <KeyType, ValueType> &now);

        void merge(int pos, block <KeyType, ValueType> &now);

        void Insert(const node <KeyType, ValueType> &tmp);

        void Delete(const node <KeyType, ValueType> &tmp);

        std::vector <ValueType> Find(const KeyType &Key);

        std::vector <ValueType> FindAll();
    };
````

### 5. 字符串类
```cpp
    class myString {
    private:
        char s[68];
        int Size;

    public:
        myString() = default;

        myString(std::string t);

        myString(const char* t);

        ~myString() = default;

        int size() { return Size; }

        char operator [](int i) { return s[i]; }

        inline bool operator <(const myString &rhs) const { return (strcmp(this->s, rhs.s) < 0); }

        inline bool operator >=(const myString &rhs) const { return !(*this < rhs); }

        inline bool operator >(const myString &rhs) const { return (strcmp(this->s, rhs.s) > 0); }

        inline bool operator <=(const myString &rhs) const { return !(*this > rhs); }

        inline bool operator ==(const myString &rhs) const { return (!strcmp(this->s, rhs.s)); }

        inline bool operator !=(const myString &rhs) const { return !(*this == rhs); }

        friend std::ostream& operator<<(std::ostream &os, myString x) { os << x.s; return os; }

        int to_int();

        double to_double();
    };
```

### 6. 错误信息类
```cpp
    class error{
    private:
        std::string msg;

    public:
        error() = default;
        error(const std::string &msg_) : msg(msg_) {}
    };
```
***
## _文件存储说明_
***
* 登录栈存储在内存中，其他信息都通过文件读写存储
* 账户、图书信息顺序存储，通过块状链表维护实现查找
* 日志内容直接顺序存储于文件即可


***
### _END_