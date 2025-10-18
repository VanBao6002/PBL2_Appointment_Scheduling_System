#pragma once
#include <string>
class User { 
    public: 
        enum class Role {ADMIN, DOCTOR, PATIENT};

    private:
        int ID;
        Role userRole;
        std::string username;
        std::string passwordHash;
        std::string permission;

    public:
        User() = default;
        ~User() = default;

    //setter
        void setID(int ID_);
        void setRole(User::Role role_);
        void setUsername(const std::string &username_);
        void setPermission(const std::string &permission_);
        void setPassword(const std::string &passwordHash_);
    
    //getter
        int getID() const {return ID;}
        Role getRole() const {return userRole;}
        const std::string& getUsername() const {return username;}
        const std::string& getPermission() const {return permission;}
        const std::string& getPassword() const {return passwordHash;}
        
};