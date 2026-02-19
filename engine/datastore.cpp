#include "datastore.h"
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

void Datastore::set(const string& key, const string& value){
    if(cache.find(key) != cache.end()){
        Node* node = cache[key];
        node->value = value;
        moveToHead(node); //Accessed most recently, tei vayera head banyo.
        return;
    }

    //Aba if nayaa nai xa provided pointer vane:

    Node* newNode =  new Node(key, value);
    cache[key] = newNode;
    addToHead(newNode);

    if(cache.size() > capacity){
        removeTail();
    }
}

string Datastore::get(const string& key){
   if(cache.find(key) == cache.end()){
    return "NULL";
   }

   Node* node = cache[key];
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

