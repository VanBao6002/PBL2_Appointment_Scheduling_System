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

bool User::loadFromStream(std::istream& is) {
    std::string line;
    if (!std::getline(is, line)) return false;
    
    std::istringstream iss(line);
    int id;
    std::string username;
    std::string password;
    int role;
    
    if (!(iss >> id >> username >> password >> role)) return false;
    
    setID(id);
    setUsername(username);
    passwordHash = password; // Đã được hash khi lưu
    userRole = static_cast<Role>(role);
    
    return true;
}

void User::saveToStream(std::ostream& os) const {
    os << getID() << " "
       << getUsername() << " "
       << getPassword() << " "
       << static_cast<int>(getRole());
}