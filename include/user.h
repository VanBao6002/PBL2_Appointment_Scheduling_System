#pragma once
#include <string>
#include <sstream>

#include "json.hpp"
#include "utils.h"

class User { 
    public: 
        enum class Role {ADMIN, MANANGER};

    private:
        int ID;
        Role userRole;
        std::string username;
        std::string passwordHash;

    public:
        User() = default;
        ~User() = default;

        void setRole(User::Role role_);
        void setUsername(const std::string &username_);
        void setPassword(const std::string &passwordHash_);
    
        int getID() const {return ID;}
        Role getRole() const {return userRole;}
        const std::string& getUsername() const {return username;}
        const std::string& getPassword() const {return passwordHash;}
        
        // convertor
        static User::Role roleFromString(const std::string& str);
        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);

    private:
        void setID(int ID_);
};