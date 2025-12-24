#pragma once

#include <unordered_map>
#include <string>
#include "utils.h"
#include <stdexcept>
#include "user.h"

class UserManager{
private:
    std::unordered_map<int, std::string> log;
    std::unordered_map<int, User> userTable;
    UserManager() = default;

public:
    static UserManager& getInstance() {
        static UserManager instance;
        return instance;
    }
    
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;
    
    ~UserManager() = default;

    void addUser(const User  &user);
    void editUser(int ID_, const User &newUser);
    void removeUser(int ID_);

    void changeRole(int ID_, const std::string& newRole);
    void changeUsername(int ID_, const std::string& newUsername);
    void changePassword(int ID_, const std::string& newPassword);
    
    const User& getUserByID(int ID_) const;
    const std::unordered_map<int, User>& getAllUsers() const; 
    const std::unordered_map<int, std::string>& getAllLog() const;
    const std::string& getIDLog(int ID_) const; 
    bool isUsernameExists(const std::string& username) const;
    
    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path);
};