#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include "json.hpp"
#include "utils.h"

class User { 
public: 
    enum class Role { ADMIN, ASSISTANT, DOCTOR };

private:
    int ID;
    Role userRole;
    std::string username;
    std::string passwordHash;

public:
    User() = default;
    User(const std::string &userRole_, const std::string &username_, const std::string &userPassword_);
    ~User() = default;

    void setRole(const std::string &role_);
    void setUsername(const std::string &username_);
    void setPassword(const std::string &passwordHash_);
    void setID(int ID_);

    int getID() const { return ID; }
    Role getRole() const { return userRole; }
    const std::string& getUsername() const { return username; }
    const std::string& getPassword() const { return passwordHash; }

    // converters
    static Role roleFromString (const std::string& str);
    static std::string roleToString (Role role);

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json &j);
};
