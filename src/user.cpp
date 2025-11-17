#include "user.h"

User::User(const std::string &userRole_, const std::string &username_, const std::string &userPassword_){

    setRole(userRole_);
    setUsername(username_);
    setPassword(userPassword_);

    int ID = IDHandler<User>::generateID();
    setID(ID);
}

void User::setID(int ID_){
    ID = ID_;
}

void User::setRole(const std::string &role_){
    std::string trimmedRole = Utils::trimmed(role_);
    Role temp = roleFromString(trimmedRole);
    userRole = temp;
}

void User::setUsername(const std::string &username_){
    std::string trimmedUsername = Utils::trimmed(username_);
    Utils::validUserName(trimmedUsername);
    username = trimmedUsername;
}   

void User::setPassword(const std::string &password_){
    std::string trimedPassword = Utils::trimmed(password_);
    Utils::validPassword(trimedPassword);
    std::string passwordHash_ = Utils::hashFunc(trimedPassword);
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