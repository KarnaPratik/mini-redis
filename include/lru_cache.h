#include<ctime>
#include<vector>
using namespace std;

#ifndef DATASTORE_H
#define DATASTORE_H

#include<unordered_map>
#include<string>

struct Node{
    string key;
    string value;
    time_t expiry;
    Node* prev;
    Node* next;

    Node(string a, string b) : key(a), value(b), expiry(0), prev(nullptr), next(nullptr){}
};

class Datastore{
    private:
        int capacity;
        int evictions = 0;
        int total_requests = 0;
        int hits = 0;
        int misses = 0; 
        unordered_map<string, Node*> cache;
        Node* head;
        Node* tail;

        void moveToHead(Node* node); //Already existing lai head ma lagne
        void removeNode(Node* node); //Node remove garne
        void addToHead(Node* node); //Naya lai head ma add garne
        void removeTail(); //Obvious one

    public:
        Datastore(int cap);
        void set(const string& key, const string& value, int ttl = 0);
        string get(const string& key);
        void del(const string& key);
        // bool exists(const string& key);

        void stats();
        int getEvictions();
        int getHits();
        int getMisses();
        vector<string> getKeys();
};


#endif