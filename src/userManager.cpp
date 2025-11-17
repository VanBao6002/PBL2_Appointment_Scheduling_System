#include "userManager.h"

void UserManager::addUser(const User  &user){
    int ID_ = user.getID();
    if (IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Adding failed. User ID " + std::to_string(user.getID()) + " already exists.");
    }
    userTable[ID_] = user;
    log[ID_] += " Added on: " + Utils::getDateTime();
    IDHandler<User>::registerID(ID_);
}

void UserManager::editUser(int ID_, const User &newUser){
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Editing failed. User ID " + std::to_string(newUser.getID()) + " not found.");
    }
    userTable[ID_] = newUser;
    log[ID_] += " Edited on " + Utils::getDateTime();
}

void UserManager::removeUser(int ID_){
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Removing failed. User ID " + std::to_string(ID_) + " not found.");
    }
    userTable.erase(ID_);
    log.erase(ID_);
    IDHandler<User>::unregisterID(ID_);
}

void UserManager::changeRole(int ID_, User::Role newRole){
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Cannot change user's role. User ID " + std::to_string(ID_) + " not found.");
    } 
    userTable[ID_].setRole(newRole);   
    log[ID_] += " Role changed on: " + Utils::getDateTime();
}

void UserManager::changeUsername(int ID_, const std::string& newUsername){
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Cannot change username. User ID " + std::to_string(ID_) + " not found.");
    }
    userTable[ID_].setUsername(newUsername);
    log[ID_] += " Username changed on " + Utils::getDateTime();
}

void UserManager::changePassword(int ID_, const std::string& newPassword){
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Cannot change password. User ID " + std::to_string(ID_) + " not found.");
    }
    userTable[ID_].setPassword(Utils::hashFunc(newPassword)); 
    log[ID_] += " Password changed on " + Utils::getDateTime();
}

const User& UserManager::getUserByID(int ID_) const {
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Failed getting. User ID " + std::to_string(ID_) + " not found.");
    }
    return userTable.at(ID_);
}

const std::unordered_map<int, User>& UserManager::getAllUsers() const {
    return userTable;
} 

const std::unordered_map<int, std::string>& UserManager::getAllLog() const {
    return log;
}

const std::string& UserManager::getIDLog(int ID_) const {
    if (!IDHandler<User>::checkDuplicate(ID_)){
        throw std::invalid_argument("Failed getting log. User ID " +  std::to_string(ID_) + " is not found.");
    }
    return log.at(ID_);
}

void UserManager::loadFromFile(const std::string& path) {
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    for (const auto& jUser : jArr) {
        User usr;
        usr.fromJson(jUser);
        addUser(usr);
    }
}


void UserManager::saveToFile(const std::string& path){
    nlohmann::json jArr;
    for (const auto& pair : userTable) {
        jArr.push_back(pair.second.toJson());
    }
    Utils::writeJsonToFile(path, jArr);
}