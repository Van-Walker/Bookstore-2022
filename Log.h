#ifndef BOOKSTORE_2022_LOG_H
#define BOOKSTORE_2022_LOG_H

#include "UnrolledLinkedList.h"
#include "Account.h"
#include <iomanip>

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

LogSystem log_sys;

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

LogSystem::LogSystem() {
    file.open(file_name);
    if (!file.good()) {
        file.open(file_name, std::fstream::out);
        file.close();
        file.open(file_name);
        log_cnt = -1;
    } else {
        file.seekg(0);
        file.read(reinterpret_cast <char *> (&log_cnt), sizeof(log_cnt));
    }
}

LogSystem::~LogSystem() {
    file.seekp(0);
    file.write(reinterpret_cast <char *> (&log_cnt), sizeof(log_cnt));
    file.close();
}

void LogSystem::Show() {
    Account user_now = acc_sys.GetUser();
    if (user_now.Privilege < 7) { throw error(); }
    double income = 0, expenditure = 0;
    log now;
    for (int pos = 0; pos <= log_cnt; ++pos) {
        file.seekg(pos * sizeof(now) + sizeof(int));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        income += now.income, expenditure += now.expenditure;
    }
    cout << std::fixed << std::setprecision(2) << "+ " << income << " - " << expenditure << "\n";
}
void LogSystem::Show(myString Count) {
    Account user_now = acc_sys.GetUser();
    if (user_now.Privilege < 7) { throw error(); }
    int count = Count.to_int();
    if (count - 1 > log_cnt) { throw error(); }
    if (count == 0) {
        cout << '\n';
        return;
    }
    double income = 0, expenditure = 0;
    log now;
    while (count--) {
        file.seekg((log_cnt - count) * sizeof(now) + sizeof(int));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        income += now.income, expenditure += now.expenditure;
    }
    cout << std::fixed << std::setprecision(2) << "+ " << income << " - " << expenditure << "\n";
}

void LogSystem::RecordLog(double income, double expenditure) {
    log now(income, expenditure);
    ++log_cnt;
    file.seekp(log_cnt * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
}

LogAll::LogAll() {
    file.open(file_name);
    if (file.good()) {
        file.seekg(0);
        file.read(reinterpret_cast <char *> (&cnt), sizeof(cnt));
    } else {
        file.open(file_name, std::fstream::out);
        file.close();
        file.open(file_name);
        cnt = -1;

    }
}

LogAll::~LogAll() {
    file.seekp(0);
    file.write(reinterpret_cast <char *> (&cnt), sizeof(cnt));
    file.close();
}

void LogAll::AddLog(Account user, char *ch) {
    ++cnt;
    Logs now(user, ch);
    file.seekp(cnt * sizeof(now) + sizeof(int));
    file.write(reinterpret_cast <char *> (&now), sizeof(now));
}

void LogAll::Log() {
    cout << "Record of " << cnt + 1 << " logs in all :\n";
    Logs now;
    for (int i = 0; i <= cnt; ++i) {
        file.seekg(i * sizeof(now) + sizeof(int));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
        if (now.user.ID != myString("")) { cout << "Account: " << now.user.ID << " "; }
        for (int j = 0; j < strlen(now.behaviour); ++j) {
            cout << now.behaviour[j];
        }
        cout  << '\n';
    }
    cout << "END.\n";
}

LogAll log_all;

#endif //BOOKSTORE_2022_LOG_H