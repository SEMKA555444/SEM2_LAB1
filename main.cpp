#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <cctype>
#include <cmath>

#include "data_structures/Array.hpp"
#include "data_structures/OneLinkedList.hpp"
#include "data_structures/DoublyLinkedList.hpp"
#include "data_structures/Stack.hpp"
#include "data_structures/Queue.hpp"
#include "data_structures/CompleteBinaryTree.hpp"

using namespace std;

struct Value {
    bool isNumber = false;
    float number = 0.0f;
    string text;
};

bool operator==(const Value& a, const Value& b) {
    if (a.isNumber != b.isNumber) return false;
    if (a.isNumber) {
        return fabs(a.number - b.number) < 1e-6f;
    }
    return a.text == b.text;
}

bool operator!=(const Value& a, const Value& b) {
    return !(a == b);
}

bool operator<(const Value& a, const Value& b) {
    if (a.isNumber && b.isNumber) {
        return a.number < b.number;
    }
    if (!a.isNumber && !b.isNumber) {
        return a.text < b.text;
    }
    return a.isNumber;
}

bool operator<=(const Value& a, const Value& b) {
    return !(b < a);
}

bool operator>(const Value& a, const Value& b) {
    return b < a;
}

bool operator>=(const Value& a, const Value& b) {
    return !(a < b);
}

ostream& operator<<(ostream& os, const Value& v) {
    if (v.isNumber) {
        os << v.number;
    } else {
        os << v.text;
    }
    return os;
}

Value parseValue(const string& s) {
    if (!s.empty()) {
        char first = s[0];
        bool looksNumeric = isdigit(static_cast<unsigned char>(first)) || first == '-' || first == '+' || first == '.';
        bool hasHex = (s.find('x') != string::npos || s.find('X') != string::npos);
        if (looksNumeric && !hasHex) {
            try {
                size_t pos = 0;
                float num = stof(s, &pos);
                if (pos == s.size() && isfinite(num)) {
                    return Value{true, num, ""};
                }
            } catch (...) {
            }
        }
    }
    return Value{false, 0.0f, s};
}

struct Database {
    map<string, Array<Value>*> arrays;
    map<string, OneLinkedList<Value>*> singleLists;
    map<string, DoublyLinkedList<Value>*> doubleLists;
    map<string, Stack<Value>*> stacks;
    map<string, Queue<Value>*> queues;
    map<string, CompleteBinaryTree<Value>*> trees;

    ~Database() {
        clearAll();
    }

    void clearAll() {
        for (auto& [_, ptr] : arrays) {
            destroy(ptr);
            delete ptr;
        }
        arrays.clear();

        for (auto& [_, ptr] : singleLists) {
            destroyList(ptr);
            delete ptr;
        }
        singleLists.clear();

        for (auto& [_, ptr] : doubleLists) {
            deleteList(ptr);
            delete ptr;
        }
        doubleLists.clear();

        for (auto& [_, ptr] : stacks) {
            clear(ptr);
            delete ptr;
        }
        stacks.clear();

        for (auto& [_, ptr] : queues) {
            clear(ptr);
            delete ptr;
        }
        queues.clear();

        for (auto& [_, ptr] : trees) {
            destroyTree(ptr);
            delete ptr;
        }
        trees.clear();
    }

    Array<Value>* getOrCreateArray(const string& name) {
        if (arrays.find(name) == arrays.end()) {
            Array<Value>* arr = new Array<Value>;
            create(arr);
            arrays[name] = arr;
        }
        return arrays[name];
    }

    OneLinkedList<Value>* getOrCreateSingleList(const string& name) {
        if (singleLists.find(name) == singleLists.end()) {
            OneLinkedList<Value>* list = new OneLinkedList<Value>;
            createList(list);
            singleLists[name] = list;
        }
        return singleLists[name];
    }

    DoublyLinkedList<Value>* getOrCreateDoubleList(const string& name) {
        if (doubleLists.find(name) == doubleLists.end()) {
            DoublyLinkedList<Value>* list = new DoublyLinkedList<Value>;
            createList(list);
            doubleLists[name] = list;
        }
        return doubleLists[name];
    }

    Stack<Value>* getOrCreateStack(const string& name) {
        if (stacks.find(name) == stacks.end()) {
            Stack<Value>* st = new Stack<Value>;
            init(st);
            stacks[name] = st;
        }
        return stacks[name];
    }

    Queue<Value>* getOrCreateQueue(const string& name) {
        if (queues.find(name) == queues.end()) {
            Queue<Value>* q = new Queue<Value>;
            initQueue(q);
            queues[name] = q;
        }
        return queues[name];
    }

    CompleteBinaryTree<Value>* getOrCreateTree(const string& name) {
        if (trees.find(name) == trees.end()) {
            CompleteBinaryTree<Value>* tree = new CompleteBinaryTree<Value>;
            initTree(tree);
            trees[name] = tree;
        }
        return trees[name];
    }
};

string toUpper(const string& s) {
    string res = s;
    for (char& c : res) {
        c = toupper(static_cast<unsigned char>(c));
    }
    return res;
}

vector<string> tokenize(const string& str) {
    vector<string> tokens;
    string current;
    bool inToken = false;
    bool inQuotes = false;
    char quoteChar = '\0';

    for (size_t i = 0; i < str.size(); ++i) {
        char c = str[i];
        if (inQuotes) {
            if (c == '\\' && i + 1 < str.size()) {
                current += str[++i];
            } else if (c == quoteChar) {
                inQuotes = false;
            } else {
                current += c;
            }
        } else {
            if (isspace(static_cast<unsigned char>(c))) {
                if (inToken) {
                    tokens.push_back(current);
                    current.clear();
                    inToken = false;
                }
            } else if (c == '"' || c == '\'') {
                inQuotes = true;
                quoteChar = c;
                inToken = true;
            } else {
                current += c;
                inToken = true;
            }
        }
    }

    if (inToken) {
        tokens.push_back(current);
    }
    return tokens;
}

string formatToken(const string& s) {
    bool needQuotes = s.empty();
    for (char c : s) {
        if (isspace(static_cast<unsigned char>(c)) || c == '"' || c == '\'' || c == '\\') {
            needQuotes = true;
            break;
        }
    }
    if (!needQuotes) return s;

    string res = "\"";
    for (char c : s) {
        if (c == '"' || c == '\\') {
            res += '\\';
        }
        res += c;
    }
    res += "\"";
    return res;
}

string formatValue(const Value& v) {
    if (v.isNumber) {
        ostringstream oss;
        oss << v.number;
        return oss.str();
    }
    return formatToken(v.text);
}

bool parseIndex(const string& s, int& idx) {
    try {
        size_t pos = 0;
        idx = stoi(s, &pos);
        return pos == s.size();
    } catch (...) {
        return false;
    }
}

bool loadDatabase(Database& db, const string& filename) {
    ifstream in(filename);
    if (!in.is_open()) {
        return false;
    }

    db.clearAll();
    string line;
    while (getline(in, line)) {
        if (line.empty() || line[0] == '#') continue;
        auto tokens = tokenize(line);
        if (tokens.size() < 2) continue;

        string type = toUpper(tokens[0]);
        string name = tokens[1];

        if (type == "M" || type == "ARRAY") {
            Array<Value>* arr = db.getOrCreateArray(name);
            for (size_t i = 2; i < tokens.size(); ++i) {
                push_back(arr, parseValue(tokens[i]));
            }
        } else if (type == "F" || type == "SLIST") {
            OneLinkedList<Value>* list = db.getOrCreateSingleList(name);
            for (size_t i = 2; i < tokens.size(); ++i) {
                push_back(list, parseValue(tokens[i]));
            }
        } else if (type == "L" || type == "DLIST") {
            DoublyLinkedList<Value>* list = db.getOrCreateDoubleList(name);
            for (size_t i = 2; i < tokens.size(); ++i) {
                push_back(list, parseValue(tokens[i]));
            }
        } else if (type == "S" || type == "STACK") {
            Stack<Value>* st = db.getOrCreateStack(name);
            for (size_t i = 2; i < tokens.size(); ++i) {
                push(st, parseValue(tokens[i]));
            }
        } else if (type == "Q" || type == "QUEUE") {
            Queue<Value>* q = db.getOrCreateQueue(name);
            for (size_t i = 2; i < tokens.size(); ++i) {
                push(q, parseValue(tokens[i]));
            }
        } else if (type == "T" || type == "TREE") {
            CompleteBinaryTree<Value>* tree = db.getOrCreateTree(name);
            for (size_t i = 2; i < tokens.size(); ++i) {
                insert(tree, parseValue(tokens[i]));
            }
        }
    }
    return true;
}

bool saveDatabase(const Database& db, const string& filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        cerr << "Error: cannot open file for writing: " << filename << endl;
        return false;
    }

    out << "# DBMS Data File" << endl;

    for (const auto& [name, arr] : db.arrays) {
        if (!arr || arr->size == 0) continue;
        out << "M " << formatToken(name);
        for (int i = 0; i < arr->size; ++i) {
            out << " " << formatValue(arr->data[i]);
        }
        out << "\n";
    }

    for (const auto& [name, list] : db.singleLists) {
        if (!list || !list->head) continue;
        out << "F " << formatToken(name);
        SNode<Value>* cur = list->head;
        while (cur) {
            out << " " << formatValue(cur->data);
            cur = cur->next;
        }
        out << "\n";
    }

    for (const auto& [name, list] : db.doubleLists) {
        if (!list || !list->head) continue;
        out << "L " << formatToken(name);
        DNode<Value>* cur = list->head;
        while (cur) {
            out << " " << formatValue(cur->data);
            cur = cur->next;
        }
        out << "\n";
    }

    for (const auto& [name, st] : db.stacks) {
        if (!st || isEmpty(st)) continue;
        vector<Value> vals;
        StackNode<Value>* cur = st->top;
        while (cur) {
            vals.push_back(cur->data);
            cur = cur->next;
        }
        reverse(vals.begin(), vals.end());
        out << "S " << formatToken(name);
        for (const Value& v : vals) {
            out << " " << formatValue(v);
        }
        out << "\n";
    }

    for (const auto& [name, q] : db.queues) {
        if (!q || isEmpty(q)) continue;
        out << "Q " << formatToken(name);
        QueueNode<Value>* cur = q->head;
        while (cur) {
            out << " " << formatValue(cur->data);
            cur = cur->next;
        }
        out << "\n";
    }

    for (const auto& [name, tree] : db.trees) {
        if (!tree || !tree->root) continue;
        out << "T " << formatToken(name);
        queue<TreeNode<Value>*> q;
        q.push(tree->root);
        while (!q.empty()) {
            TreeNode<Value>* cur = q.front();
            q.pop();
            out << " " << formatValue(cur->data);
            if (cur->left) q.push(cur->left);
            if (cur->right) q.push(cur->right);
        }
        out << "\n";
    }

    return true;
}

bool executeQuery(Database& db, const string& queryStr, bool& modified) {
    auto tokens = tokenize(queryStr);
    if (tokens.empty()) return true;

    string cmd = toUpper(tokens[0]);

    if (cmd == "PRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: PRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.trees.find(name) != db.trees.end()) {
            cout << "Tree [" << name << "]:" << endl;
            printTree(db.trees[name]);
            return true;
        }
        if (db.arrays.find(name) != db.arrays.end()) {
            cout << "Array [" << name << "]: ";
            print(db.arrays[name]);
            return true;
        }
        if (db.singleLists.find(name) != db.singleLists.end()) {
            cout << "SinglyLinkedList [" << name << "]:" << endl;
            printList(db.singleLists[name]);
            return true;
        }
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            cout << "DoublyLinkedList [" << name << "]:" << endl;
            printList(db.doubleLists[name]);
            return true;
        }
        if (db.stacks.find(name) != db.stacks.end()) {
            cout << "Stack [" << name << "]:" << endl;
            print(db.stacks[name]);
            return true;
        }
        if (db.queues.find(name) != db.queues.end()) {
            cout << "Queue [" << name << "]:" << endl;
            print(db.queues[name]);
            return true;
        }
        cout << "Structure '" << name << "' not found" << endl;
        return false;
    }

    if (cmd == "SPUSH") {
        if (tokens.size() < 3) {
            cerr << "Usage: SPUSH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        Stack<Value>* st = db.getOrCreateStack(name);
        push(st, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "SPOP") {
        if (tokens.size() < 2) {
            cerr << "Usage: SPOP <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.stacks.find(name) == db.stacks.end() || isEmpty(db.stacks[name])) {
            cout << "Stack is empty" << endl;
            return false;
        }
        Stack<Value>* st = db.stacks[name];
        Value val = top(st);
        pop(st);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "STOP" || cmd == "SGET") {
        if (tokens.size() < 2) {
            cerr << "Usage: STOP <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.stacks.find(name) == db.stacks.end() || isEmpty(db.stacks[name])) {
            cout << "Stack is empty" << endl;
            return false;
        }
        cout << "-> " << top(db.stacks[name]) << endl;
        return true;
    }
    if (cmd == "SSIZE") {
        if (tokens.size() < 2) {
            cerr << "Usage: SSIZE <name>" << endl;
            return false;
        }
        string name = tokens[1];
        int sz = (db.stacks.find(name) != db.stacks.end()) ? size(db.stacks[name]) : 0;
        cout << "-> " << sz << endl;
        return true;
    }
    if (cmd == "SPRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: SPRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.stacks.find(name) == db.stacks.end()) {
            cout << "Stack is empty" << endl;
            return false;
        }
        print(db.stacks[name]);
        return true;
    }

    if (cmd == "QPUSH") {
        if (tokens.size() < 3) {
            cerr << "Usage: QPUSH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        Queue<Value>* q = db.getOrCreateQueue(name);
        push(q, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "QPOP") {
        if (tokens.size() < 2) {
            cerr << "Usage: QPOP <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.queues.find(name) == db.queues.end() || isEmpty(db.queues[name])) {
            cout << "Queue is empty" << endl;
            return false;
        }
        Queue<Value>* q = db.queues[name];
        Value val = front(q);
        pop(q);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "QFRONT" || cmd == "QGET") {
        if (tokens.size() < 2) {
            cerr << "Usage: QFRONT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.queues.find(name) == db.queues.end() || isEmpty(db.queues[name])) {
            cout << "Queue is empty" << endl;
            return false;
        }
        cout << "-> " << front(db.queues[name]) << endl;
        return true;
    }
    if (cmd == "QSIZE") {
        if (tokens.size() < 2) {
            cerr << "Usage: QSIZE <name>" << endl;
            return false;
        }
        string name = tokens[1];
        int sz = (db.queues.find(name) != db.queues.end()) ? size(db.queues[name]) : 0;
        cout << "-> " << sz << endl;
        return true;
    }
    if (cmd == "QPRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: QPRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.queues.find(name) == db.queues.end()) {
            cout << "Queue is empty" << endl;
            return false;
        }
        print(db.queues[name]);
        return true;
    }

    if (cmd == "MPUSH" || cmd == "MPUSH_BACK") {
        if (tokens.size() < 3) {
            cerr << "Usage: MPUSH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        Array<Value>* arr = db.getOrCreateArray(name);
        push_back(arr, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "MINSERT") {
        if (tokens.size() < 4) {
            cerr << "Usage: MINSERT <name> <index> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        Value val = parseValue(tokens[3]);
        Array<Value>* arr = db.getOrCreateArray(name);
        if (idx < 0 || idx > arr->size) {
            cerr << "Index out of range" << endl;
            return false;
        }
        insert(arr, idx, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "MGET") {
        if (tokens.size() < 3) {
            cerr << "Usage: MGET <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.arrays.find(name) == db.arrays.end()) {
            cerr << "Array not found" << endl;
            return false;
        }
        Array<Value>* arr = db.arrays[name];
        if (idx < 0 || idx >= arr->size) {
            cerr << "Index out of range" << endl;
            return false;
        }
        cout << "-> " << arr->data[idx] << endl;
        return true;
    }
    if (cmd == "MDEL") {
        if (tokens.size() < 3) {
            cerr << "Usage: MDEL <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.arrays.find(name) == db.arrays.end()) {
            cerr << "Array not found" << endl;
            return false;
        }
        Array<Value>* arr = db.arrays[name];
        if (idx < 0 || idx >= arr->size) {
            cerr << "Index out of range" << endl;
            return false;
        }
        erase(arr, idx);
        modified = true;
        cout << "-> OK" << endl;
        return true;
    }
    if (cmd == "MSET") {
        if (tokens.size() < 4) {
            cerr << "Usage: MSET <name> <index> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        Value val = parseValue(tokens[3]);
        if (db.arrays.find(name) == db.arrays.end()) {
            cerr << "Array not found" << endl;
            return false;
        }
        Array<Value>* arr = db.arrays[name];
        if (idx < 0 || idx >= arr->size) {
            cerr << "Index out of range" << endl;
            return false;
        }
        change(arr, idx, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "MLEN" || cmd == "MSIZE") {
        if (tokens.size() < 2) {
            cerr << "Usage: MLEN <name>" << endl;
            return false;
        }
        string name = tokens[1];
        int len = (db.arrays.find(name) != db.arrays.end()) ? length(db.arrays[name]) : 0;
        cout << "-> " << len << endl;
        return true;
    }
    if (cmd == "MPRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: MPRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.arrays.find(name) != db.arrays.end()) {
            print(db.arrays[name]);
        }
        return true;
    }

    if (cmd == "FPUSH" || cmd == "FPUSH_BACK") {
        if (tokens.size() < 3) {
            cerr << "Usage: FPUSH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        OneLinkedList<Value>* list = db.getOrCreateSingleList(name);
        push_back(list, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "FPUSH_FRONT") {
        if (tokens.size() < 3) {
            cerr << "Usage: FPUSH_FRONT <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        OneLinkedList<Value>* list = db.getOrCreateSingleList(name);
        push_front(list, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "FINSERT_BEFORE" || cmd == "FINSERT_FW") {
        if (tokens.size() < 4) {
            cerr << "Usage: FINSERT_BEFORE <name> <index> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        Value val = parseValue(tokens[3]);
        OneLinkedList<Value>* list = db.getOrCreateSingleList(name);
        insertForward(list, idx, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "FINSERT_AFTER" || cmd == "FINSERT_BW") {
        if (tokens.size() < 4) {
            cerr << "Usage: FINSERT_AFTER <name> <index> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        Value val = parseValue(tokens[3]);
        OneLinkedList<Value>* list = db.getOrCreateSingleList(name);
        insertBackward(list, idx, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "FPOP_FRONT") {
        if (tokens.size() < 2) {
            cerr << "Usage: FPOP_FRONT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.singleLists.find(name) != db.singleLists.end()) {
            pop_front(db.singleLists[name]);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "FPOP_BACK") {
        if (tokens.size() < 2) {
            cerr << "Usage: FPOP_BACK <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.singleLists.find(name) != db.singleLists.end()) {
            pop_back(db.singleLists[name]);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "FDEL" || cmd == "FDEL_VAL") {
        if (tokens.size() < 3) {
            cerr << "Usage: FDEL <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.singleLists.find(name) != db.singleLists.end()) {
            deletingByValue(db.singleLists[name], val);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "FDEL_FW" || cmd == "FDEL_BEFORE") {
        if (tokens.size() < 3) {
            cerr << "Usage: FDEL_BEFORE <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.singleLists.find(name) != db.singleLists.end()) {
            removeForward(db.singleLists[name], idx);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "FDEL_BW" || cmd == "FDEL_AFTER") {
        if (tokens.size() < 3) {
            cerr << "Usage: FDEL_AFTER <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.singleLists.find(name) != db.singleLists.end()) {
            removeBackward(db.singleLists[name], idx);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "FGET") {
        if (tokens.size() < 3) {
            cerr << "Usage: FGET <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.singleLists.find(name) != db.singleLists.end()) {
            SNode<Value>* n = getAt(db.singleLists[name], idx);
            if (n) {
                cout << "-> " << n->data << endl;
                return true;
            }
        }
        cerr << "Element not found" << endl;
        return false;
    }
    if (cmd == "FSEARCH") {
        if (tokens.size() < 3) {
            cerr << "Usage: FSEARCH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.singleLists.find(name) != db.singleLists.end()) {
            searchByValue(db.singleLists[name], val);
            return true;
        }
        return false;
    }
    if (cmd == "FLEN" || cmd == "FSIZE") {
        if (tokens.size() < 2) {
            cerr << "Usage: FLEN <name>" << endl;
            return false;
        }
        string name = tokens[1];
        int len = (db.singleLists.find(name) != db.singleLists.end()) ? length(db.singleLists[name]) : 0;
        cout << "-> " << len << endl;
        return true;
    }
    if (cmd == "FPRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: FPRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.singleLists.find(name) != db.singleLists.end()) {
            printList(db.singleLists[name]);
        }
        return true;
    }
    if (cmd == "FPRINT_REV") {
        if (tokens.size() < 2) {
            cerr << "Usage: FPRINT_REV <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.singleLists.find(name) != db.singleLists.end()) {
            printListReverse(db.singleLists[name]);
        }
        return true;
    }
    if (cmd == "FPRINT_ASC") {
        if (tokens.size() < 2) {
            cerr << "Usage: FPRINT_ASC <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.singleLists.find(name) != db.singleLists.end()) {
            printListAscending(db.singleLists[name]);
        }
        return true;
    }
    if (cmd == "FPRINT_DESC") {
        if (tokens.size() < 2) {
            cerr << "Usage: FPRINT_DESC <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.singleLists.find(name) != db.singleLists.end()) {
            printListDescending(db.singleLists[name]);
        }
        return true;
    }

    if (cmd == "LPUSH" || cmd == "LPUSH_BACK") {
        if (tokens.size() < 3) {
            cerr << "Usage: LPUSH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        DoublyLinkedList<Value>* list = db.getOrCreateDoubleList(name);
        push_back(list, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "LPUSH_FRONT") {
        if (tokens.size() < 3) {
            cerr << "Usage: LPUSH_FRONT <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        DoublyLinkedList<Value>* list = db.getOrCreateDoubleList(name);
        push_front(list, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "LINSERT_BEFORE" || cmd == "LINSERT_FW") {
        if (tokens.size() < 4) {
            cerr << "Usage: LINSERT_BEFORE <name> <index> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        Value val = parseValue(tokens[3]);
        DoublyLinkedList<Value>* list = db.getOrCreateDoubleList(name);
        insertForward(list, idx, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "LINSERT_AFTER" || cmd == "LINSERT_BW") {
        if (tokens.size() < 4) {
            cerr << "Usage: LINSERT_AFTER <name> <index> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        Value val = parseValue(tokens[3]);
        DoublyLinkedList<Value>* list = db.getOrCreateDoubleList(name);
        insertBackward(list, idx, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "LPOP_FRONT") {
        if (tokens.size() < 2) {
            cerr << "Usage: LPOP_FRONT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            pop_front(db.doubleLists[name]);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "LPOP_BACK") {
        if (tokens.size() < 2) {
            cerr << "Usage: LPOP_BACK <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            pop_back(db.doubleLists[name]);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "LDEL" || cmd == "LDEL_VAL") {
        if (tokens.size() < 3) {
            cerr << "Usage: LDEL <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            deletingByValue(db.doubleLists[name], val);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "LDEL_FW" || cmd == "LDEL_BEFORE") {
        if (tokens.size() < 3) {
            cerr << "Usage: LDEL_BEFORE <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            removeForward(db.doubleLists[name], idx);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "LDEL_BW" || cmd == "LDEL_AFTER") {
        if (tokens.size() < 3) {
            cerr << "Usage: LDEL_AFTER <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            removeBackward(db.doubleLists[name], idx);
            modified = true;
            cout << "-> OK" << endl;
            return true;
        }
        return false;
    }
    if (cmd == "LGET") {
        if (tokens.size() < 3) {
            cerr << "Usage: LGET <name> <index>" << endl;
            return false;
        }
        string name = tokens[1];
        int idx = 0;
        if (!parseIndex(tokens[2], idx)) {
            cerr << "Invalid index: " << tokens[2] << endl;
            return false;
        }
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            DNode<Value>* n = getAt(db.doubleLists[name], idx);
            if (n) {
                cout << "-> " << n->data << endl;
                return true;
            }
        }
        cerr << "Element not found" << endl;
        return false;
    }
    if (cmd == "LSEARCH") {
        if (tokens.size() < 3) {
            cerr << "Usage: LSEARCH <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            searchByValue(db.doubleLists[name], val);
            return true;
        }
        return false;
    }
    if (cmd == "LLEN" || cmd == "LSIZE") {
        if (tokens.size() < 2) {
            cerr << "Usage: LLEN <name>" << endl;
            return false;
        }
        string name = tokens[1];
        int len = (db.doubleLists.find(name) != db.doubleLists.end()) ? length(db.doubleLists[name]) : 0;
        cout << "-> " << len << endl;
        return true;
    }
    if (cmd == "LPRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: LPRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            printList(db.doubleLists[name]);
        }
        return true;
    }
    if (cmd == "LPRINT_REV") {
        if (tokens.size() < 2) {
            cerr << "Usage: LPRINT_REV <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            printListReverse(db.doubleLists[name]);
        }
        return true;
    }
    if (cmd == "LPRINT_ASC") {
        if (tokens.size() < 2) {
            cerr << "Usage: LPRINT_ASC <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            printListAscending(db.doubleLists[name]);
        }
        return true;
    }
    if (cmd == "LPRINT_DESC") {
        if (tokens.size() < 2) {
            cerr << "Usage: LPRINT_DESC <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.doubleLists.find(name) != db.doubleLists.end()) {
            printListDescending(db.doubleLists[name]);
        }
        return true;
    }

    if (cmd == "TINSERT") {
        if (tokens.size() < 3) {
            cerr << "Usage: TINSERT <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        CompleteBinaryTree<Value>* tree = db.getOrCreateTree(name);
        insert(tree, val);
        modified = true;
        cout << "-> " << val << endl;
        return true;
    }
    if (cmd == "TSEARCH" || cmd == "ISMEMBER") {
        if (tokens.size() < 3) {
            cerr << "Usage: ISMEMBER <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.trees.find(name) == db.trees.end()) {
            cout << "-> FALSE" << endl;
            return true;
        }
        bool found = search(db.trees[name], val);
        cout << (found ? "-> TRUE" : "-> FALSE") << endl;
        return true;
    }
    if (cmd == "TISCOMPLETE" || cmd == "TCHECK") {
        if (tokens.size() < 2) {
            cerr << "Usage: TISCOMPLETE <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.trees.find(name) == db.trees.end()) {
            cout << "-> TRUE" << endl;
            return true;
        }
        bool ok = isComplete(db.trees[name]);
        cout << (ok ? "-> TRUE" : "-> FALSE") << endl;
        return true;
    }
    if (cmd == "TDEL") {
        if (tokens.size() < 2) {
            cerr << "Usage: TDEL <name> [value]" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.trees.find(name) == db.trees.end()) {
            cout << "Tree not found" << endl;
            return false;
        }

        if (tokens.size() == 2) {
            bool ok = isComplete(db.trees[name]);
            cout << (ok ? "-> TRUE" : "-> FALSE") << endl;
            return true;
        } else {
            Value val = parseValue(tokens[2]);
            bool res = remove(db.trees[name], val);
            if (res) {
                modified = true;
                cout << "-> OK" << endl;
            } else {
                cout << "-> NOT_FOUND" << endl;
            }
            return res;
        }
    }
    if (cmd == "TGET") {
        if (tokens.size() < 2) {
            cerr << "Usage: TGET <name> [inorder|preorder|postorder|bfs]" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.trees.find(name) == db.trees.end()) {
            cout << "(empty tree)" << endl;
            return true;
        }
        string order = (tokens.size() >= 3) ? toUpper(tokens[2]) : "INORDER";
        if (order == "PREORDER" || order == "PRE") {
            printPreOrder(db.trees[name]);
        } else if (order == "POSTORDER" || order == "POST") {
            printPostOrder(db.trees[name]);
        } else if (order == "BFS" || order == "LEVEL") {
            printBFS(db.trees[name]);
        } else {
            printInOrder(db.trees[name]);
        }
        return true;
    }
    if (cmd == "TPRINT") {
        if (tokens.size() < 2) {
            cerr << "Usage: TPRINT <name>" << endl;
            return false;
        }
        string name = tokens[1];
        if (db.trees.find(name) != db.trees.end()) {
            printTree(db.trees[name]);
        } else {
            cout << "(empty tree)" << endl;
        }
        return true;
    }
    if (cmd == "TSIZE") {
        if (tokens.size() < 2) {
            cerr << "Usage: TSIZE <name>" << endl;
            return false;
        }
        string name = tokens[1];
        int sz = (db.trees.find(name) != db.trees.end()) ? size(db.trees[name]) : 0;
        cout << "-> " << sz << endl;
        return true;
    }
    if (cmd == "TSUCC") {
        if (tokens.size() < 3) {
            cerr << "Usage: TSUCC <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.trees.find(name) != db.trees.end()) {
            Value succ;
            if (findSuccessor(db.trees[name], val, succ)) {
                cout << "-> " << succ << endl;
                return true;
            }
        }
        cout << "No successor found" << endl;
        return false;
    }
    if (cmd == "TPRED") {
        if (tokens.size() < 3) {
            cerr << "Usage: TPRED <name> <value>" << endl;
            return false;
        }
        string name = tokens[1];
        Value val = parseValue(tokens[2]);
        if (db.trees.find(name) != db.trees.end()) {
            Value pred;
            if (findPredecessor(db.trees[name], val, pred)) {
                cout << "-> " << pred << endl;
                return true;
            }
        }
        cout << "No predecessor found" << endl;
        return false;
    }

    cerr << "Unknown command: " << tokens[0] << endl;
    return false;
}

int main(int argc, char* argv[]) {
    string filename = "";
    string query = "";

    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        } else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
    }

    if (filename.empty()) {
        cerr << "Usage: " << argv[0] << " --file <filename> [--query '<command>']" << endl;
        return 1;
    }

    Database db;
    loadDatabase(db, filename);

    if (!query.empty()) {
        bool modified = false;
        bool ok = executeQuery(db, query, modified);
        if (modified) {
            saveDatabase(db, filename);
        }
        return ok ? 0 : 1;
    }

    cout << "=== Simple DBMS CLI ===" << endl;
    cout << "Loaded database file: " << filename << endl;
    cout << "Type queries or 'EXIT' / 'QUIT' to leave." << endl;

    string line;
    while (true) {
        cout << "dbms> ";
        if (!getline(cin, line)) break;
        if (line.empty()) continue;

        string trimmed = line;
        while (!trimmed.empty() && isspace(static_cast<unsigned char>(trimmed.front()))) trimmed.erase(trimmed.begin());
        while (!trimmed.empty() && isspace(static_cast<unsigned char>(trimmed.back()))) trimmed.pop_back();

        string upperLine = toUpper(trimmed);
        if (upperLine == "EXIT" || upperLine == "QUIT") {
            break;
        }

        bool modified = false;
        executeQuery(db, trimmed, modified);
        if (modified) {
            saveDatabase(db, filename);
        }
    }

    return 0;
}
