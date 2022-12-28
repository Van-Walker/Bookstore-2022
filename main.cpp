#include "TokenScanner.h"

int main() {
    std::ios::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
    string str;
    char ch[1005];
    bool flag = true;
    int cnt_ = 0;
    myString root("root");
    if (acc_sys.ID_to_pos(root) == -1) {
        acc_sys.Register(myString("root"), myString("sjtu"), myString("root"), 7);
    }
    while (flag) {
        if (!getline(cin, str)) { break; }
        if (str.size() > 1000) { cout << "Invalid\n"; }
        else {
            strcpy(ch, str.c_str());
            Work(ch, flag);
        }
    }
    return 0;
}