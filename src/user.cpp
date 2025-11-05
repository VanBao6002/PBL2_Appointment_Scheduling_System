#include "user.h"
#include "utils.h"
#include <string>

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

void User::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&ID), sizeof(ID));

    int roleInt = static_cast<int>(userRole);
    os.write(reinterpret_cast<const char*>(&roleInt), sizeof(roleInt));

    size_t usernameLen = username.size();
    os.write(reinterpret_cast<const char*>(&usernameLen), sizeof(usernameLen));
    os.write(username.c_str(), usernameLen);

    size_t passwordHashLen = passwordHash.size();
    os.write(reinterpret_cast<const char*>(&passwordHashLen), sizeof(passwordHashLen));
    os.write(passwordHash.c_str(), passwordHashLen);
}

void User::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&ID), sizeof(ID));

    int roleInt;
    is.read(reinterpret_cast<char*>(&roleInt), sizeof(roleInt));
    userRole = static_cast<User::Role>(roleInt);

    size_t usernameLen;
    is.read(reinterpret_cast<char*>(&usernameLen), sizeof(usernameLen));
    username.resize(usernameLen);
    is.read(&username[0], usernameLen);

    size_t passwordHashLen;
    is.read(reinterpret_cast<char*>(&passwordHashLen), sizeof(passwordHashLen));
    passwordHash.resize(passwordHashLen);
    is.read(&passwordHash[0], passwordHashLen);
}
