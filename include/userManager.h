#pragma once

#include "user.h"
#include <unordered_map>
#include <string>
#include "serializable.h"

class UserManager : public ISerializable {
    private:
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, User> userTable;

    public:
        UserManager() = default;
        ~UserManager() = default;

        void addUser(int ID_, const User  &user_);
        void editUser(int ID_, const User &newUser);
        void removeUser(int ID_);

        void changeRole(int ID_, User::Role newRole);
        void changeUsername(int ID_, const std::string& newUsername);
        void changePassword(int ID_, const std::string& newPassword);
        
        const User& getUserByID(int ID_) const;
        const std::unordered_map<int, User>& getAllUsers() const; 
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

        // File operations
        void loadFromFile(const std::string& filename);
        void saveToFile(const std::string& filename) const;
};