#include "user.h"

User::User(const std::string &userRole_, const std::string &username_, const std::string &userPassword_) {
    setRole(userRole_);
    setUsername(username_);
    setPassword(userPassword_);

    // gán đúng vào thuộc tính class
    ID = IDHandler<User>::generateID();
}

void User::setID(int ID_) {
    ID = ID_;
}

void User::setRole(const std::string &role_){
    userRole = roleFromString(Utils::trimmed(role_));
}

void User::setUsername(const std::string &username_){
    Utils::validUserName(Utils::trimmed(username_));
    username = Utils::trimmed(username_);
}   

void User::setPassword(const std::string &password_){
    Utils::validPassword(Utils::trimmed(password_));
    passwordHash = Utils::hashFunc(Utils::trimmed(password_));
}

User::Role User::roleFromString(const std::string& str) {
    std::string lower = str;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "admin") return Role::ADMIN;
    if (lower == "assistant") return Role::ASSISTANT;
    if (lower == "doctor") return Role::DOCTOR;

    throw std::invalid_argument("Unknown role: " + str);
}

std::string User::roleToString(Role role) {
    switch (role) {
        case Role::ADMIN:     return "ADMIN";
        case Role::ASSISTANT: return "ASSISTANT";
        case Role::DOCTOR:    return "DOCTOR";
    }
    return "UNKNOWN";
}

nlohmann::json User::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["userRole"] = roleToString(userRole);
    j["username"] = username;
    j["passwordHash"] = passwordHash;
    return j;
}

void User::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("userRole")) userRole = roleFromString(j.at("userRole").get<std::string>());
    if (j.contains("username")) username = j.at("username").get<std::string>();
    if (j.contains("passwordHash")) passwordHash = j.at("passwordHash").get<std::string>();
}