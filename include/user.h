#pragma once
#include <string>
#include <sstream>
#include <algorithm>
#include "json.hpp"
#include "utils.h"
#include "date.h"

class User { 
public: 
    enum class Role {ADMIN, ASSISTANT};

private:
    int ID;
    Role userRole;
    std::string username;
    std::string passwordHash;
    std::string plainPassword;
    std::string fullName;
    std::string phoneNumber;
    std::string CCCD;
    std::string birthday;
    Date creationDate;

public:
    User() = default;
    User(const std::string &userRole_, const std::string &username_, const std::string &userPassword_,
        const std::string &fullName_, const std::string &cccd_, const std::string &phone_, const std::string &birthday_);
    ~User();

    User(const User& other);
    User& operator=(const User& other);
    User(User&& other) noexcept;
    User& operator=(User&& other) noexcept;
    
    void setRole(const std::string &role_);
    void setUsername(const std::string &username_);
    void setPassword(const std::string &passwordHash_);
    void setPlainPassword(const std::string& plainPwd);
    void setID(int ID_);
    void setFullName(const std::string &name);
    void setPhoneNumber(const std::string &phone);
    void setCCCD(const std::string &cccd);
    void setBirthday(const std::string &bd);
    void setCreationDate(const Date& date);

    int getID() const { return ID; }
    Role getRole() const { return userRole; }
    const std::string& getUsername() const { return username; }
    const std::string& getPassword() const { return passwordHash; }
    const std::string& getPlainPassword() const { return plainPassword; }
    const std::string& getFullName() const { return fullName; }
    const std::string& getPhoneNumber() const { return phoneNumber; }
    const std::string& getCCCD() const { return CCCD; }
    const std::string& getBirthday() const { return birthday; }
    const Date& getCreationDate() const { return creationDate; }

    // converters
    static Role roleFromString (const std::string& str);
    static std::string roleToString (Role role);

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json &j);
};