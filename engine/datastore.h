using namespace std;

#ifndef DATASTORE_H
#define DATASTORE_H

#include<unordered_map>
#include<string>

class Datastore{
    private:
        unordered_map<string, string> store;

    public:
        void set(const string& key, const string& value);
        string get(const string& key);
        void del(const string& key);
        bool exists(const string& key);
};


#endif