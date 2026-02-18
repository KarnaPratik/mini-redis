#include<iostream>
#include<sstream>
#include "datastore.h"
using namespace std;

int main(){
    Datastore db;
    string input;

    cout << "Mini Redis v1\n";

    while(true){
        cout << "> ";
        getline(cin, input);

        stringstream ss(input);
        string command;
        ss >> command;

        if(command == "SET" || command == "set" || command == "Set"){
            string key, value;
            ss >> key >> value;
            db.set(key, value);
            cout << "Ok\n";
        }else if(command == "GET" || command == "get" || command == "Get"){
            string key;
            ss >> key;
            cout << db.get(key) << "\n";
        }else if(command == "DEL" || command == "del" || command == "Del"){
            string key;
            ss >> key;
            db.del(key);
            cout << "Deleted\n";
        }else if(command == "EXISTS" || command == "exists" || command == "Exists"){
            string key;
            ss >> key;
            db.exists(key);
        }else if(command == "EXIT" || command == "exit" || command =="Exit"){
            break;
        }else{
            cout << "Invalid command\n";
        }
    }

    return 0;
}
