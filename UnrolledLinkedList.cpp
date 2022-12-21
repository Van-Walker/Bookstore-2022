#include <iostream>
#include <fstream>
#include <cstring>
//#include "UnrolledLinkedList.h"

const int MAXN = 1e5 + 5;
const int SQRN = 350; // sqrt(n)

class myString {
private:
    char s[68];
public:
    myString() = default;
    myString(const char* t) {
        int len = strlen(t);
        if (len > 64) exit(-1);
        for (register int i = 0; i < len; ++i) {
            s[i] = t[i];
        }
        s[len] = '\0';
    }
    ~myString() = default;

    inline bool operator <(const myString &rhs) const {
        return (strcmp(this->s, rhs.s) < 0);
    }

    inline bool operator >=(const myString &rhs) const {
        return !(*this < rhs);
    }

    inline bool operator >(const myString &rhs) const {
        return (strcmp(this->s, rhs.s) > 0);
    }

    inline bool operator <=(const myString &rhs) const {
        return !(*this > rhs);
    }

    inline bool operator ==(const myString &rhs) const {
        return (!strcmp(this->s, rhs.s));
    }

    inline bool operator !=(const myString &rhs) const {
        return !(*this == rhs);
    }

    std::ostream& operator<<(std::ostream &os) {
        os << this->s;
        return os;
    }
};

template <class KeyType, class ValueType>
struct node {
    KeyType Key;
    ValueType Value;
    node() = default;

    node(const KeyType &key, const ValueType &value) {
        Key = key, Value = value;
    }

    inline bool operator ==(const node &rhs) const {
        if (Key == rhs.Key && Value == rhs.Value) { return true; }
        return false;
    }

    inline bool operator !=(const node &rhs) const {
        return !(*this == rhs);
    }

    inline bool operator <(const node &rhs) const {
        if (Key != rhs.Key) { return Key < rhs.Key; }
        return Value < rhs.Value;
    }

    inline bool operator >=(const node &rhs) const {
        return !(*this < rhs);
    }

    inline bool operator >(const node &rhs) const {
        if (Key != rhs.Key) { return Key > rhs.Key; }
        return Value > rhs.Value;
    }

    inline bool operator <=(const node &rhs) const {
        return !(*this > rhs);
    }
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
    const std::string file_name = "test_ull";
    const int max_cnt = 2 * SQRN;
    const int min_cnt = SQRN;
    int block_cnt = -1;

public:
    UnrolledLinkedList() {
        file.open(file_name);
        if (file.good()) {
            file.seekg(0);
            file.read(reinterpret_cast <char *> (&block_cnt), sizeof(int));
        } else {
            file.open(file_name, std::fstream::out);
            file.close();
            file.open(file_name);
            block_cnt = -1;
            file.seekp(0);
            file.write(reinterpret_cast <char *> (&block_cnt), sizeof(int));
            file.seekp(sizeof(int));
            static block <KeyType, ValueType> temp;
            file.write(reinterpret_cast <char *> (&temp), sizeof(temp));
        }
    }

    ~UnrolledLinkedList() {
        file.seekp(0);
        file.write(reinterpret_cast <char*> (&block_cnt), sizeof(int));
        file.close();
    }

    void ReadBlock(int pos, block <KeyType, ValueType> &now) {
        if (pos < 0) { exit(-1); }
        file.seekg(pos * sizeof(now) + sizeof(int));
        file.read(reinterpret_cast <char *> (&now), sizeof(now));
    }

    void ReadLittle(int pos, block <KeyType, ValueType> &now) {
        if (pos < 0) { exit(-1); }
        file.seekg(pos * sizeof(now) + sizeof(int));
        file.read(reinterpret_cast <char *> (&now.min_node), sizeof(now.min_node));
        file.read(reinterpret_cast <char *> (&now.max_node), sizeof(now.max_node));
        file.read(reinterpret_cast <char *> (&now.cnt), sizeof(now.cnt));
        file.read(reinterpret_cast <char *> (&now.next), sizeof(now.next));
    }

    void WriteBlock(int pos, block <KeyType, ValueType> &now) {
        if (pos < 0) { exit(-1); }
        file.seekp(pos * sizeof(now) + sizeof(int));
        file.write(reinterpret_cast <char *> (&now), sizeof(now));
    }

    void split(int pos, block <KeyType, ValueType> &now) {
        static block <KeyType, ValueType> tmp;
        for (register int i = min_cnt; i < now.cnt; ++i) {
            tmp.list[i - min_cnt] = now.list[i];
        }
        ++block_cnt;
        tmp.cnt = now.cnt - min_cnt;
        tmp.next = now.next;
        tmp.min_node = tmp.list[0];
        tmp.max_node = tmp.list[tmp.cnt - 1];
        now.cnt = min_cnt;
        now.next = block_cnt;
        now.min_node = now.list[0];
        now.max_node = now.list[now.cnt - 1];
        WriteBlock(pos, now);
        WriteBlock(block_cnt, tmp);
    }

    void merge(int pos, block <KeyType, ValueType> &now) {
        static block <KeyType, ValueType> tmp;
        if (now.next == -1) {
            WriteBlock(pos, now);
            return;
        }
        ReadLittle(now.next, tmp);
        if (now.cnt + tmp.cnt > max_cnt) {
            WriteBlock(pos, now);
            return;
        }
        ReadBlock(now.next, tmp);
        for (register int i = now.cnt; i < now.cnt + tmp.cnt; ++i) {
            now.list[i] = tmp.list[i - now.cnt];
        }
        now.cnt += tmp.cnt;
        now.next = tmp.next;
        now.min_node = now.list[0];
        now.max_node = now.list[now.cnt - 1];
        WriteBlock(pos, now);
    }

    void Insert(const node <KeyType, ValueType> &tmp) {
        static block<KeyType, ValueType> now;
        if (block_cnt == -1) {
            block_cnt = 0;
            now.cnt = 1;
            now.min_node = tmp;
            now.max_node = tmp;
            now.list[0] = tmp;
            WriteBlock(0, now);
            return;
        }
        int pos = 0;
        while (true) {
            ReadLittle(pos, now);
            if (tmp <= now.max_node && now.cnt != 0) {
                ReadBlock(pos, now);
                for (register int i = 0; i < now.cnt; ++i) {
                    if (tmp == now.list[i]) { return; }
                    if (tmp < now.list[i]) {
                        for (register int j = now.cnt; j > i; --j) {
                            now.list[j] = now.list[j - 1];
                        }
                        ++now.cnt;
                        now.list[i] = tmp;
                        now.min_node = now.list[0];
                        now.max_node = now.list[now.cnt - 1];
                        if (now.cnt > max_cnt) {
                            split(pos, now);
                            return;
                        } else {
                            WriteBlock(pos, now);
                            return;
                        }
                    }
                }
            }
            if (now.next == -1) { break; }
            pos = now.next;
        }
        // 插到最后一块
        ReadBlock(pos, now);
        ++now.cnt;
        now.list[now.cnt - 1] = tmp;
        now.min_node = now.list[0];
        now.max_node = now.list[now.cnt - 1];
        if (now.cnt > max_cnt) {
            split(pos, now);
            return;
        } else { WriteBlock(pos, now); }
    }

    void Delete(const node <KeyType, ValueType> &tmp) {
        if (block_cnt == -1) { return; }
        static block <KeyType, ValueType> now;
        int pos = 0;
        while (pos != -1) {
            ReadLittle(pos, now);
            if (now.cnt == 0) {
                pos = now.next;
                continue;
            }
            if (tmp >= now.min_node && tmp <= now.max_node) {
                ReadBlock(pos, now);
                for (register int i = 0; i < now.cnt; ++i) {
                    if (now.list[i] > tmp) { return; }
                    if (now.list[i] == tmp) {
                        for (int j = i + 1; j < now.cnt; ++j) {
                            now.list[j - 1] = now.list[j];
                        }
                        --now.cnt;
                        if (now.cnt) {
                            now.min_node = now.list[0];
                            now.max_node = now.list[now.cnt - 1];
                        }
                        if (now.cnt < min_cnt) { merge(pos, now); }
                        else { WriteBlock(pos, now); }
                        return;
                    }
                }
                return;
            } else if (tmp < now.min_node) { return; }
            pos = now.next;
        }
    }

    void Find(const KeyType &Key) {
        static block <KeyType, ValueType> now;
        int pos = 0;
        bool flag = false;
        while (pos != -1) {
            ReadLittle(pos, now);
            if (now.cnt == 0) {
                pos = now.next;
                continue;
            }
            if (Key <= now.max_node.Key && Key >= now.min_node.Key) {
                ReadBlock(pos, now);
                for (register int i = 0; i < now.cnt; ++i) {
                    if (now.list[i].Key == Key) {
                        std::cout << now.list[i].Value << " ";
                        flag = true;
                    }
                }
            } else if (Key < now.min_node.Key) { break; }
            pos = now.next;
        }
        if (flag) { std::cout << "\n"; }
        else { std::cout << "null\n"; }
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(0);
    std::cout.tie(0);
    UnrolledLinkedList <myString, int> ull;
    int T;
    scanf("%d", &T);
    while (T--) {
        char stat[70];
        int val;
        scanf("%s", stat);
        if (strcmp(stat, "insert") == 0) {
            scanf("%s", stat);
            scanf("%d", &val);
            node <myString, int> tmp(myString(stat), val);
            ull.Insert(tmp);
        } else if (strcmp(stat, "delete") == 0) {
            scanf("%s", stat);
            scanf("%d", &val);
            node <myString, int> tmp(myString(stat), val);
            ull.Delete(tmp);
        } else if (strcmp(stat, "find") == 0) {
            scanf("%s", stat);
            ull.Find(myString(stat));
        }
    }
    return 0;
}