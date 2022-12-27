#ifndef BOOKSTORE_2022_LOG_H
#define BOOKSTORE_2022_LOG_H

#include "UnrolledLinkedList.h"

struct log {
    double income;
    double expenditure;
    log() = default;
    log(double x, double y) {
        income = x, expenditure = y;
    }
};

class LogSystem { // todo
private:
    int log_cnt;
    std::fstream file;
    string file_name = "log";
public:
    LogSystem() {
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

    ~LogSystem() {
        file.seekp(0);
        file.write(reinterpret_cast <char *> (&log_cnt), sizeof(log_cnt));
        file.close();
    }

    void Show() {
        double income = 0, expenditure = 0;
        log now;
        for (int pos = 0; pos <= log_cnt; ++pos) {
            file.seekg(pos * sizeof(now) + sizeof(int));
            file.read(reinterpret_cast <char *> (&now), sizeof(now));
            income += now.income, expenditure += now.expenditure;
        }
    }

    void Show(myString Count) {
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
        cout << "+ " << std::fixed << std::setprecision(2) << income << " - " << expenditure << "\n";
    }

    void RecordLog(double income, double expenditure) {
        log now(income, expenditure);
        ++log_cnt;
        file.seekp(log_cnt * sizeof(now) + sizeof(int));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
    }

    void Log() {
        // todo
    }
};

LogSystem log_sys;


#endif //BOOKSTORE_2022_LOG_H
