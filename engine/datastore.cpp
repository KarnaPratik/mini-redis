#include "datastore.h"
using namespace std;

void Datastore::set(const string& key, const string& value){
    store[key] = value;
}

string Datastore::get(const string& key){
    if (store.find(key) != store.end()){
        return store[key];
    }

    return "NULL";
}

void Datastore::del(const string& key){
    store.erase(key);
}

bool Datastore::exists(const string& key){
    return store.find(key) != store.end();
}

