#include "user.h"

void User::setID(int ID_){
    Utils::validID(ID_);
    ID = ID_;
}

void User::setRole(User::Role role_){
    userRole = role_;
}

void User::setUsername(const std::string &username_){
    Utils::validUserName(username_);
    username = username_;
}   

void User::setPassword(const std::string &password_){
    Utils::validPassword(password_);
    std::string passwordHash_ = Utils::hashFunc(password_);
    passwordHash = passwordHash_;
}

User::Role User::roleFromString(const std::string& str) {
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "admin") return User::Role::ADMIN;
    if (lowerStr == "manager") return User::Role::MANANGER;
    throw std::invalid_argument("Unknown status: " + str);
}

nlohmann::json User::toJson() const {
    nlohmann::json j;
        j["ID"] = ID;
        j["userRole"] = userRole;
        j["username"] = username;
        j["passwordHash"] = passwordHash;
        return j;
}

void User::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("userRole")) userRole = roleFromString(j.at("userRole").get<std::string>());
    if (j.contains("username")) username = j.at("username").get<std::string>();
    if (j.contains("passwordHash")) username = j.at("passwordHash").get<std::string>();
}