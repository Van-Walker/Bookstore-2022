# Bookstore
### _ACM 2022 Big Homework_
###### _Writen by Vanius / wt_
***
## _程序功能概述_
***
### 1. 命令行交互
### 2. 账户系统
>* 信息：用户名、 ID、 密码、 登录状态    
>* 操作：注册账户（游客）、 创建账户（员工）、 登录账户、 修改密码、 注销账户、 删除账户   
### 3. 图书系统
>* 信息：ISBN号、 书名、 作者名、 关键词（可多个）、 单价（可改变）、 购买数量   
>* 操作：根据 ISBN号 / 书名 / 作者名 / 关键词 检索图书  
>* 记录购买图书  
>* 修改图书信息  
>* 图书进货：以指定交易总额购入指定数量的选中图书  
### 4. 日志系统
>* 保存交易记录  
>* 查询最后完成的指定笔数交易总额  
>* 生成日志
### 5. 合法判断
>* 判断每条输入是否合法，不合法应报错并放弃执行
***

## _主题逻辑说明_ 
***
>* 在 main 函数中从控制台循环读入指令，依次处理
>* 针对当前读入指令，先确定类型，并识别明显不合法的指令（第一类错误抛出）
>* 按照类型执行各类指令，并执行第二类错误抛出
>* 将各类操作执行完后，文件输出到相应的储存文件保存修改，并记录日志

***
## _各个类的接口及成员说明_
***
### 1. 指令类
    class StatementInput {
    private:
        std::string statement;
    
    public:
        StatementInput();
        StatementInput(string &stat);
        ~StatementInput();
        StatementJudge();
    };
        

### 2. 书店类
    class Bookstore {
    private:
        AccountSystem acc_sys;
        BookSystem book_sys;
        LoginSystem login_sys; // 登录
        FinanceLogSystem fin_sys;
        LogSystem log_sys; // 日志
    public:
        Bookstore();
        ~Bookstore();
        Quit();
        Exit();
        
        // 用户系统：
        void Su(const std::string &id, const std::string &password);
        void Logout();
        void Register(const std::string &id, const std::string &password, const std::string &name);
        void Passwd(const std::string &id, const std::string &current_password, const std::string new_password);
        void Useradd(const std::string &id, const std::string &password, const std::string &priviledge, const std::string &name);
        void Delete(const std::string &id);        

        // 图书系统：
        void Show(const std::string &information);
        void Buy(const std::string &ISBN, const std::string &quantity);
        void Select(const std::string &ISBN);
        void Modify(const std::string &information);
        void Import(const std::string &quantity, const std::string &totalcost);
        
        // 日志系统：
        ShowFinance(const std::string &count);
        ShowLog();
    };

### 3. 账户类
    struct UserID_ {
        char ID[31];
        UserID_();
        UserID_(const std::string &s);
        ~UserID_();
        UserID_ &operator =(const UserID_ &rhs);
        inline bool operator ==(const UserID_ &rhs) const;
        inline bool operator <(const UserID_ &rhs) const;
    };
    
    struct Account_ {
        UserID_ ID;
        char Password[31], Name[31];
        int Priviledge;
        Account_();
        Account_(const std::string &id, const std::string &password, const std::string &name, const std::string &priviledge);
        ~Account_();
        void EditPassword(const std::string &cur_password, const std::string &new_password);
    };
    
    class AccountSystem {
    private:
        UnrolledLinkedList <UserID_, int> id_index;
        std::fstream accountsystem;        

    public:
        AccountSystem();
        ~AccountSystem();
        void Su(const std::string &id, const std::string &password);
        void Register(const std::string &id, const std::string &password, const std::string &name);
        void Passwd(const std::string &id, const std::string &current_password, const std::string new_password);
        void Useradd(const std::string &id, const std::string &password, const std::string &priviledge, const std::string &name);
        void Delete(const std::string &id); 
        void EditPassword(const std::string &current_password, const std::string new_password);
        Account_ FindAccount(const std::string &id);
        Account_ FindAccount(const UserID_ &id);
    };

### 4. 图书类
    struct ISBN_ {
        char[21] ISBN;
        ISBN_(const std::string &tmpISBN);
        ISBN_ &operator=(const ISBN_ &rhs);
        inline bool operator ==(const ISBN_ &rhs) const;
        inline bool operator <(const ISBN_ &rhs) const;
    };

    struct Name_ {
        char[61] Name;
        Name_(const std::string &tmpName);
        Name_ &operator=(const Name_ &rhs);
        inline bool operator ==(const Name_ &rhs) const;
        inline bool operator <(const Name_ &rhs) const;
    };

    struct Author_ {
        char[61] Author;
        Author_(const std::string &tmpAuthor);
        Author_ &operator=(const Author_ &rhs);
        inline bool operator ==(const Author_ &rhs) const;
        inline bool operator <(const Author_ &rhs) const;
    };

    struct Keyword_ {
        char[61] Keyword;
        Keyword_(const std::string &tmpKeyword);
        Keyword_ &operator=(const Keyword_ &rhs);
        inline bool operator ==(const Keyword_ &rhs) const;
        inline bool operator <(const Keyword_ &rhs) const;
    };
    
    struct Book_ {
        ISBN_ ISBN;
        Name_ Name;
        Author_ Author;
        Keyword_ Keyword;
        int Quantity;
        double Price;
        double Cost;
        Book();
        Book(const std::string &isbn, const std::string &name, const std::string &author, const std::string &keyword, int quantity, double price, double cost);
        Book(const std::string &isbn);
        ~Book();
    };

    class BookSystem {
    private:
        UnrolledLinkedList <ISBN_, int> ISBN_index;
        UnrolledLinkedList <Name_, int> id_index;
        UnrolledLinkedList <Author_, int> id_index;
        UnrolledLinkedList <Keyword_, int> id_index;
        std::fstream booksystem;
    
    public:
        BookSystem();
        ~BookSystem();
        void Show(const std::string &information);
        void Buy(const std::string &isbn, const std::string &num);
        void Select(const std::string &isbn);
        void Modify(const std::string &information);
        void Import(const std::string &num, const std::string &cost);
    };

### 5. 日志类 
    struct FinanceLog_ {
        char Stat[21];
        Book_ Book;
        int Quantity;
        double Sum;
        bool Sgn;
        FinanceLog();
        FinanceLog(const std::string &statement, const Book_ &book, const int &num, const double &sum);
        ~FinanceLog();
    };
    
    class FinanceLogSystem {
    private:
        std::fstream financelogsystem;
    
    public:
        FinanceLogSystem();
        ~FinanceLogSystem();
        void ShowFinance(const int &count);
    };

    struct Log {
        char stat[200];
        UserID_ ID;
        int Quantity;
        double Sum;
        bool Sgn;
    };

    class LogSystem {
    private:
        std::fstream logsystem;

    public:
        LogSystem();
        ~LogSystem();
        void Log();
    };

### 6. 登录类
    class LoginSystem {
    private:
        std::vector <std::string> ID;
        std::vector <int> Priviledge;
        
    public:
        LoginSystem();
        ~LoginSystem();
        void LogIn();
        void LogOut
    };

### 7. 块状链表类
    template <class KeyType, class ValueType>
    class UnrolledLinkedList {
    private:
        std::fstream File;
        int Size;
    
    public:
        UnrolledLinkedList(const std::string& file, int size_);
        ~UnrolledLinkedList();
        void Insert(const KeyType &key, const ValueType &value);
        void Erase(const KeyType &key);
        void Modify(const KeyType &key, const ValueType &value);
        void Clear();
        ValueType* GetValue(const KeyType &key);
        std::vector<valueType> traverse();
    };

***
## _文件存储说明_
***
>* 登录栈存储在内存中，其他信息都通过文件读写存储
>* 账户、图书信息顺序存储，通过块状链表维护实现查找
>* 日志内容直接顺序操作即可

***
## _其他补充说明_
***
>* 待续

***
### _END_