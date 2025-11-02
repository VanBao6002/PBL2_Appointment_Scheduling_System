#include "userManager.h"
#include "user.h"
#include "utils.h"

void UserManager::addUser(int ID_, const User  &user_){
    if (userTable.find(ID_) != userTable.end()) {
        throw std::invalid_argument("user ID already exists.");
    }
    userTable[ID_] = user_;
    log[ID_] += " Added on " + Utils::getDateTime();
}

void UserManager::editUser(int ID_, const User &newUser){
    Utils::validUserID(userTable, ID_);
    userTable[ID_] = newUser;
    log[ID_] += " Edited on " + Utils::getDateTime();
}

void UserManager::removeUser(int ID_){
    Utils::validUserID(userTable, ID_);
    userTable.erase(ID_);
    log.erase(ID_);
}

void UserManager::changeRole(int ID_, User::Role newRole){
    Utils::validUserID(userTable, ID_); // Validate user exists
    userTable[ID_].setRole(newRole);    // Change role using User's setter
    log[ID_] += " Role changed on " + Utils::getDateTime();
}

void UserManager::changeUsername(int ID_, const std::string& newUsername){
    Utils::validUserID(userTable, ID_);
    userTable[ID_].setUsername(newUsername);
    log[ID_] += " Username changed on " + Utils::getDateTime();
}

void UserManager::changePassword(int ID_, const std::string& newPassword){
    Utils::validUserID(userTable, ID_);
    userTable[ID_].setPassword(Utils::hashFunc(newPassword)); // Consider hashing here
    log[ID_] += " Password changed on " + Utils::getDateTime();
}

const User& UserManager::getUserByID(int ID_) const {
    Utils::validUserID(userTable, ID_);
    return userTable.at(ID_);
}

const std::unordered_map<int, User>& UserManager::getAllUsers() const {
    return userTable;
} 

const std::unordered_map<int, std::string>& UserManager::getAllLog() const {
    return log;
}

const std::string& UserManager::getIDLog(int ID_) const {
    Utils::validUserID(userTable, ID_);
    return log.at(ID_);
}

void UserManager::loadFromFile(const std::string& filename) {
    Utils::loadFromFile(filename, userTable);
}

void UserManager::saveToFile(const std::string& filename) const {
    Utils::saveToFile(filename, userTable);
}

bool UserManager::loadFromStream(std::istream& is) {
    return Utils::loadFromStream(is, userTable);
}

void UserManager::saveToStream(std::ostream& os) const {
    Utils::saveToStream(os, userTable);
}