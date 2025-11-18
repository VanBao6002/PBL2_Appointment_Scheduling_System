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
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "admin") return User::Role::ADMIN;
    if (lowerStr == "assistant") return User::Role::ASSISTANT;
    if (lowerStr == "doctor") return User::Role::DOCTOR;

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
    if (j.contains("passwordHash")) passwordHash = j.at("passwordHash").get<std::string>();
}