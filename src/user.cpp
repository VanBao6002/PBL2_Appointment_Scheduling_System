#include "user.h"
#include "utils.h"
#include <string>

void User::setID(int ID_){
    Utils::validID(ID_);
    ID = ID_;
}

void User::setRole(User::Role role_){
    userRole = role_;
}

void User::setUsername(const std::string &username_){
    Utils::validUserName(username_);
    username = username_;
}   

void User::setPassword(const std::string &password_){
    Utils::validPassword(password_);
    std::string passwordHash_ = Utils::hashFunc(password_);
    passwordHash = passwordHash_;
}