using namespace std;

#ifndef DATASTORE_H
#define DATASTORE_H

#include<unordered_map>
#include<string>

struct Node{
    string key;
    string value;
    Node* prev;
    Node* next;

    Node(string a, string b) : key(a), value(b), prev(nullptr), next(nullptr){}
};

class Datastore{
    private:
        int capacity;
        unordered_map<string, Node*> cache;
        Node* head;
        Node* tail;

        void moveToHead(Node* node);
        void removeNode(Node* node);
        void addToHead(Node* node);
        void removeTail();

    public:
        Datastore(int cap);
        void set(const string& key, const string& value);
        string get(const string& key);
        void del(const string& key);
        bool exists(const string& key);
};


#endif