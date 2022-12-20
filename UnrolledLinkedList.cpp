#include <iostream>
#include <fstream>
#include <cstring>
//#include "UnrolledLinkedList.h"

const int MAXN = 1e5 + 5;
const int SQRN = 350; // sqrt(n)

template <class KeyType, class ValueType>
struct node {
public:
    KeyType Key;
    ValueType Value;
    node() = default;

    node(KeyType key, ValueType value) {
        Key = key, Value = value;
    }
    ~node() = default;

    bool operator ==(const node &rhs) const {
        if (Key == rhs.Key && Value == rhs.Value) { return true; }
        return false;
    }

    bool operator !=(const node &rhs) const {
        return !(this == rhs);
    }

    bool operator <(const node &rhs) const {
        if (Key != rhs.Key) { return Key < rhs.Key; }
        return Value < rhs.Value;
    }

    bool operator >=(const node &rhs) const {
        return !(this < rhs);
    }

    bool operator >(const node &rhs) const {
        if (Key != rhs.Key) { return Key > rhs.Key; }
        return Value > rhs.Value;
    }

    bool operator <=(const node &rhs) const {
        return !(this > rhs);
    }
};

template <class KeyType, class ValueType>
struct block {
    node <KeyType, ValueType> max_node, min_node;
    int cnt = 0;
    int next = -1;
    node <KeyType, ValueType> list[2 * SQRN + 5];

};

template <class KeyType, class ValueType>
class UnrolledLinkedList {
private:
    std::fstream file;
    std::string file_name = "test";
    const int max_cnt = 2 * SQRN;
    const int min_cnt = SQRN;
    int block_cnt = -1;


    // todo


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
            file.seekp(0);
            file.write(reinterpret_cast <char *> (&block_cnt), sizeof(int));
            file.seekp(4);
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
        for (int i = min_cnt; i < now.cnt; ++i) {
            tmp.list[i - SQRN] = now.list[i];
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
        if (now.cnt + tmp.next > max_cnt) {
            WriteBlock(pos, now);
            return;
        }
        ReadBlock(now.next, tmp);
        for (int i = now.cnt; i < now.cnt + tmp.cnt; ++i) {
            now.list[i] = tmp.list[i - now.cnt];
        }
        now.cnt += tmp.cnt;
        now.next = tmp.next;
        now.min_node = now.list[0];
        now.max_node = now.list[now.cnt - 1];
        WriteBlock(pos, now);
    }

    void Insert(const node <KeyType, ValueType> &tmp) {
        // todo

    }

    void Delete(const node <KeyType, ValueType> &tmp) {
        // todo
    }

    void Find(const KeyType Key) {
        // todo
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
                for (int i = 0; i < now.cnt; ++i) {
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
    UnrolledLinkedList <std::string, int> ull;
    int T;
    std::cin >> T;
    while (--T) {
        char stat[70];
        int val;
        scanf("%s", &stat);
        if (strcmp(stat, "insert") == 0) {
            scanf("%s", &stat);
            scanf("%d", &val);
            node <std::string, int> tmp(std::string(stat), val);
            ull.Insert(tmp);
        } else if (strcmp(stat, "delete") == 0) {
            scanf("%s", &stat);
            scanf("%d", &val);
            node <std::string, int> tmp(std::string(stat), val);
            ull.Delete(tmp);
        } else if (strcmp(stat, "find") == 0) {
            scanf("%s", &stat);
            ull.Find(std::string(stat));
        }
    }
    return 0;
}