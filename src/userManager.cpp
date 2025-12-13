#include "userManager.h"
#include "utils.h"  
#include "IDHandler.h"
#include <qlogging.h>
#include <QDebug>
#include <stdexcept> 

void UserManager::addUser(const User  &user){
    int ID_ = user.getID();
    if (userTable.find(ID_) != userTable.end()){
        throw std::invalid_argument("Adding failed. User ID " + std::to_string(user.getID()) + " already exists.");
    }
    userTable[ID_] = user;
    log[ID_] += " Added on: " + Utils::getDateTime();
}

void UserManager::editUser(int ID_, const User &newUser){
    if (userTable.find(ID_) == userTable.end()){
        throw std::invalid_argument("Editing failed. User ID " + std::to_string(newUser.getID()) + " not found.");
    }
    userTable[ID_] = newUser;
    log[ID_] += " Edited on " + Utils::getDateTime();
}

void UserManager::removeUser(int ID_){
    if (userTable.find(ID_) == userTable.end()){
        throw std::invalid_argument("Removing failed. User ID " + std::to_string(ID_) + " not found.");
    }
    userTable.erase(ID_);
    log.erase(ID_);
}

void UserManager::changeRole(int ID_, const std::string& newRole){
    if (userTable.find(ID_) == userTable.end()){
        throw std::invalid_argument("Cannot change user's role. User ID " + std::to_string(ID_) + " not found.");
    } 
    userTable[ID_].setRole(newRole);   
    log[ID_] += " Role changed on: " + Utils::getDateTime();
}

void UserManager::changeUsername(int ID_, const std::string& newUsername){
    if (userTable.find(ID_) == userTable.end()){
        throw std::invalid_argument("Cannot change username. User ID " + std::to_string(ID_) + " not found.");
    }
    userTable[ID_].setUsername(newUsername);
    log[ID_] += " Username changed on " + Utils::getDateTime();
}

void UserManager::changePassword(int ID_, const std::string& newPassword){
    if (userTable.find(ID_) == userTable.end()){
        throw std::invalid_argument("Cannot change password. User ID " + std::to_string(ID_) + " not found.");
    }
    // ✅ Truyền password plain, không hash
    userTable[ID_].setPassword(newPassword); 
    log[ID_] += " Password changed on " + Utils::getDateTime();
}

const User& UserManager::getUserByID(int ID_) const {
    if (userTable.find(ID_) == userTable.end()){
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
    if (log.find(ID_) == log.end()){
        throw std::invalid_argument("Failed getting log. User ID " +  std::to_string(ID_) + " is not found.");
    }
    return log.at(ID_);
}

void UserManager::loadFromFile(const std::string& path) {
    // clean data before loading
    userTable.clear();
    log.clear();
    IDHandler<User>::resetIDTable(); 

    // check active path, propriate data
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] User file is empty or invalid format";
        return;
    }

    // start reading and load to memory
    int maxID = 0;
    for (const auto& jUser : jArr) {
    try {
        User u;
        u.fromJson(jUser);
        int ID = u.getID();
        if (userTable.count(ID)) {
            qWarning() << "[WARNING] Duplicate User ID in file:" << ID << "- Skipping";
            continue;
        }
        if (!IDHandler<User>::checkDuplicateID(static_cast<size_t>(ID))) {
            IDHandler<User>::registerID(static_cast<size_t>(ID));
        }
        userTable[ID] = u;
        if (ID > maxID) maxID = ID;
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Failed to load User record:" << e.what();
    }
    }
    
    // Set current ID > maxID
    if (maxID >= 0) {
        IDHandler<User>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << userTable.size() << "Users from file";
}


void UserManager::saveToFile(const std::string& path){
     try {
        nlohmann::json jArr;
        for (const auto& pair : userTable) {
            jArr.push_back(pair.second.toJson());
        }        
        Utils::writeJsonToFile(path, jArr);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save doctors data: " << e.what() << std::endl;
        throw; // rethrow for caller (UI layer) show message
    }
}
