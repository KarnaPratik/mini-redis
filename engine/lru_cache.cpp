#include "datastore.h"
#include<iostream>
using namespace std;

Datastore::Datastore(int cap){
    capacity = cap;
    head = nullptr;
    tail = nullptr;
}

void Datastore::addToHead(Node* node){
    node->next = head;
    node->prev = nullptr;

    //Doubly linked list ho tei vayera node lai head banaaune, ani head lai pani node point garna laaune. 

    if(head){
        head->prev = node;
    }

    head = node;

    if(!tail){
        tail = head;
    }
}


void Datastore::removeNode(Node* node){
    if(node->prev){
        node->prev->next = node->next;
    }else{
        head = node->next;
    }

   if(node->next){
    node->next->prev = node->prev;
   }else{
    tail = node->prev;
   }
    
   //Euta node remove garepaxi duitai previous ra next ko pointer haru update garne to the next corresponding element. 
}

void Datastore::moveToHead(Node* node){
    removeNode(node);
    addToHead(node);
}

void Datastore::removeTail(){
    if(!tail) return;

    Node* temp = tail;
    removeNode(temp);
    cache.erase(temp->key);
    delete temp;
}

void Datastore::set(const string& key, const string& value, int ttl){
    if(cache.find(key) != cache.end()){
        Node* node = cache[key];
        node->value = value;

        if(ttl > 0){ //Does the user acutally want it to expire? Maarinde nai ho tyo key vaako value lai?
            node->expiry = time(nullptr) + ttl;
        }else{
            node->expiry = 0; // the node was resuced, Aba mardaina, CPR diyo.
        }


        moveToHead(node); //Accessed most recently, tei vayera head banyo.
        return;
    }

    //Aba if nayaa nai xa provided pointer vane:

    Node* newNode =  new Node(key, value);

    if(ttl > 0) newNode->expiry = time(nullptr) + ttl;

    cache[key] = newNode;
    addToHead(newNode);

    if(cache.size() > capacity){
        removeTail();
        evictions++;
    }
}

string Datastore::get(const string& key){

    total_requests++;

   if(cache.find(key) == cache.end()){
    return "NULL";
   }

   Node* node = cache[key];


   //Lazy expiry use vayo i.e. access navaako vaye tyo ttl ko matlab nai hunthena, access garisakepaxi maatrar key expire hunxa. Etti kam time ma etti maatra lekhan sakinxa, fancy way ma heap banaayera entire cache scan garna aaudaina malai.
   if(node->expiry != 0 && time(nullptr) > node->expiry){
    removeNode(node);
    cache.erase(key);
    delete node;
    misses++;
    return "NULL";
   }


   hits++;
   addToHead(node);
   return node->value;
}

void Datastore::del(const string& key){
    if(cache.find(key) == cache.end()) return;

    Node* node = cache[key];
    removeNode(node);
    cache.erase(key);
    delete node;
}

// bool Datastore::exists(const string& key){
//     return store.find(key) != store.end();
// }

void Datastore::stats(){
    cout << "Total Requests: " << total_requests << endl;
    cout << "Total hits: " << hits << endl;
    cout << "Total misses: " << misses << endl;

    if(total_requests > 0){
        cout << "Hit rate: " << (double)hits / total_requests * 100 << "%" << endl;
    }

    cout << "Total evictions: " << evictions << endl;
    cout << "Total no of keys: " << cache.size() << endl;
}

