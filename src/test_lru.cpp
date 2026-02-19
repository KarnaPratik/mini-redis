#include<iostream>
#include<sstream>
#include "lru_cache.h"
using namespace std;

int main(){
    Datastore db(3);
    string input;

    cout << "Mini Redis v1\n";

    while(true){
        cout << "> ";
        getline(cin, input);

        stringstream ss(input);
        string command;
        ss >> command;

        if(command == "SET" || command == "set" || command == "Set"){
            string key, value, exp;
            ss >> key >> value >> exp;

            if(exp == "EX"){
                int ttl;
                ss >> ttl;
                db.set(key, value, ttl);
            }else{
                db.set(key, value);
            }

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
         }//else if(command == "EXISTS" || command == "exists" || command == "Exists"){
        //     string key;
        //     ss >> key;
        //     db.exists(key);
        // }
        else if(command == "EXIT" || command == "exit" || command =="Exit"){
            break;
        }else if(command == "STATS" || command == "stats" || command == "Stats"){
            db.stats();
        }else{
            cout << "Invalid command\n";
        }
    }

    return 0;
}
