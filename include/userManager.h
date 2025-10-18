#pragma once

#include "user.h"
#include <unordered_map>
#include <string>

class UserManager{
    private:
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, User> userTable;

    public:
        UserManager() = default;
        ~UserManager() = default;

        // modifiers
        void addUser(int ID_, const User  &user_);
        void editUser(int ID_, const User &newUser);
        void removeUser(int ID_);

        void changeRole(int ID_, User::Role newRole);
        void changeUsername(int ID_, const std::string& newUsername);
        void changePassword(int ID_, const std::string& newPassword);
        
        // getters
        const User& getUserByID(int ID_) const;
        const std::unordered_map<int, User>& getAllUsers() const; 
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

        
};