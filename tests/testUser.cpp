#include "all.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
using namespace std;

void TEST_PRINT_ALL_INFO (UserManager& mgr) {
    for (const auto& pair : mgr.getAllUsers()) {
        cout << "ID: " << pair.second.getID() << ", Name: " << pair.second.getUsername() << endl;
    }
}

void TEST_LOAD(UserManager& mgr, const string& path){
    mgr.loadFromFile(path);
}

void TEST_ADD(UserManager& mgr, User& doc) {
    mgr.addUser(doc);
}

void TEST_EDIT(UserManager& mgr, User& doc) {
    mgr.editUser(doc.getID(), doc);
}

void TEST_REMOVE(UserManager& mgr, User& doc) {
    mgr.removeUser(doc.getID());
}



int main (){
    UserManager mgr;

    TEST_LOAD(mgr, Config::USER_PATH);

    User d1;
    d1.setUsername("timmy123");
    d1.setPassword("111111111");
    TEST_ADD(mgr, d1);
    TEST_PRINT_ALL_INFO(mgr);
    

    d1.setUsername("timmy686868");
    d1.setPassword("222222222");
    TEST_EDIT(mgr, d1);
    TEST_PRINT_ALL_INFO(mgr);


    TEST_REMOVE(mgr, d1);
    TEST_PRINT_ALL_INFO(mgr);



    
}