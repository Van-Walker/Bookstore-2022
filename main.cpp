#include "TokenScanner.h"

void Init() {
    myString sjtu("root");
    if (acc_sys.ID_to_pos(sjtu) != -1) {
        acc_sys.Register(myString("root"), myString("sjtu"), myString("root"), 7);
    }
    // todo: Init of Log
}

int main() {
    string str;
    char ch[1005];
    bool flag = true;
    Init();
    while (flag) {
        if (!getline(cin, str)) { break; }
        if (str.size() > 1000) { cout << "Invalid\n"; }
        else {
            strcpy(ch, str.c_str());
            Work(ch, flag);
        }
    }
    // todo: log记录并输出
    return 0;
}