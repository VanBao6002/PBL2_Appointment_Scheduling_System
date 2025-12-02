#include "user.h"
#include "appointment.h"
#include "IDHandler.h"

User::User(const std::string &userRole_, const std::string &username_, const std::string &userPassword_) {
    setRole(userRole_);
    setUsername(username_);
    
    // Lưu cả plain và hash
    std::string trimmedPwd = Utils::trimmed(userPassword_);
    Utils::validPassword(trimmedPwd);
    
    plainPassword = trimmedPwd;                // Lưu plain để hiển thị
    passwordHash = Utils::hashFunc(trimmedPwd); // Lưu hash để xác thực
    
    int ID = static_cast<int>(IDHandler<User>::generateID());
    setID(ID);
    IDHandler<User>::registerID(ID);
}

User::~User(){
    if (ID != 0) {  // Chỉ unregister nếu ID hợp lệ
        IDHandler<User>::unregisterID(ID);
    }
}

// Copy Constructor - Copy ID, do not generate/register new one
User::User(const User& other)
    : ID(other.ID),
      userRole(other.userRole),
      username(other.username),
      passwordHash(other.passwordHash)
{
    // Do not register ID again (ownership stays with original)
}

// Copy Assignment Operator - Copy fields, do not register/unregister ID
User& User::operator=(const User& other)
{
    if (this != &other) {
        ID = other.ID;
        userRole = other.userRole;
        username = other.username;
        passwordHash = other.passwordHash;
        // Do not register/unregister ID
    }
    return *this;
}

// Move Constructor - Move fields, transfer ID, clear other's ID
User::User(User&& other) noexcept
    : ID(other.ID),
      userRole(std::move(other.userRole)),
      username(std::move(other.username)),
      passwordHash(std::move(other.passwordHash))
{
    other.ID = 0;
}

// Move Assignment Operator - Move fields, transfer ID, clear other's ID
User& User::operator=(User&& other) noexcept
{
    if (this != &other) {
        ID = other.ID;
        userRole = std::move(other.userRole);
        username = std::move(other.username);
        passwordHash = std::move(other.passwordHash);
        other.ID = 0;
    }
    return *this;
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
    std::string trimmedPwd = Utils::trimmed(password_);
    Utils::validPassword(trimmedPwd);
    
    plainPassword = trimmedPwd;                // Lưu plain để hiển thị
    passwordHash = Utils::hashFunc(trimmedPwd); // Lưu hash để xác thực
}

User::Role User::roleFromString (const std::string& str) {
    if (Utils::toLower(str) == "admin") return Role::ADMIN;
    if (Utils::toLower(str) == "assistant") return Role::ASSISTANT;

    throw std::invalid_argument("Unknown role: " + str);
}

std::string User::roleToString (Role role) {
    switch (role) {
        case Role::ADMIN:     return "ADMIN";
        case Role::ASSISTANT: return "ASSISTANT";
    }
    return "UNKNOWN";
}

nlohmann::json User::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["userRole"] = roleToString(userRole);
    j["username"] = username;
    j["passwordHash"] = passwordHash;  // Vẫn lưu hash để bảo mật
    j["plainPassword"] = plainPassword; // Lưu plain để hiển thị (có thể bỏ nếu không muốn)
    return j;
}

void User::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("userRole")) userRole = roleFromString(j.at("userRole").get<std::string>());
    if (j.contains("username")) username = j.at("username").get<std::string>();
    if (j.contains("passwordHash")) passwordHash = j.at("passwordHash").get<std::string>();
    
    // THÊM: đọc plainPassword nếu có, nếu không thì để trống
    if (j.contains("plainPassword")) {
        plainPassword = j.at("plainPassword").get<std::string>();
    } else {
        plainPassword = "";  // Nếu file cũ không có plainPassword
    }
}
