#include "user.h"
#include "appointment.h"
#include "IDHandler.h"

User::User(const std::string &userRole_, const std::string &username_, const std::string &userPassword_,
           const std::string &fullName_, const std::string &cccd_, const std::string &phone_, 
           const std::string &birthday_) {
    setRole(userRole_);
    setUsername(username_);
    
    std::string trimmedPwd = Utils::trimmed(userPassword_);
    Utils::validPassword(trimmedPwd);
    
    plainPassword = trimmedPwd;
    passwordHash = Utils::hashFunc(trimmedPwd);
    
    int ID = static_cast<int>(IDHandler<User>::generateID());
    setID(ID);
    IDHandler<User>::registerID(ID);

    fullName = Utils::trimmed(fullName_);
    CCCD = Utils::trimmed(cccd_);
    phoneNumber = Utils::trimmed(phone_);
    birthday = Utils::trimmed(birthday_);
    
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm* now_tm = std::localtime(&now_c);
    
    Date currentDate(now_tm->tm_mday, now_tm->tm_mon + 1, now_tm->tm_year + 1900);
    setCreationDate(currentDate);
}

User::~User(){
    if (ID != 0) {
        IDHandler<User>::unregisterID(ID);
    }
}

User::User(const User& other)
    : ID(other.ID),
      userRole(other.userRole),
      username(other.username),
      passwordHash(other.passwordHash),
      plainPassword(other.plainPassword),
      fullName(other.fullName),         
      phoneNumber(other.phoneNumber),   
      CCCD(other.CCCD),                  
      birthday(other.birthday),         
      creationDate(other.creationDate)   
{
    
}

User& User::operator=(const User& other)
{
    if (this != &other) {
        ID = other.ID;
        userRole = other.userRole;
        username = other.username;
        passwordHash = other.passwordHash;
        plainPassword = other.plainPassword;
        fullName = other.fullName;
        phoneNumber = other.phoneNumber;
        CCCD = other.CCCD;
        birthday = other.birthday;
        creationDate = other.creationDate;
    }
    return *this;
}

User::User(User&& other) noexcept
    : ID(other.ID),
      userRole(std::move(other.userRole)),
      username(std::move(other.username)),
      passwordHash(std::move(other.passwordHash)),
      plainPassword(std::move(other.plainPassword)),
      fullName(std::move(other.fullName)),         
      phoneNumber(std::move(other.phoneNumber)),  
      CCCD(std::move(other.CCCD)),                   
      birthday(std::move(other.birthday)),           
      creationDate(std::move(other.creationDate))
{
    other.ID = 0;
}

User& User::operator=(User&& other) noexcept
{
    if (this != &other) {
        ID = other.ID;
        userRole = std::move(other.userRole);
        username = std::move(other.username);
        passwordHash = std::move(other.passwordHash);
        plainPassword = std::move(other.plainPassword);
        fullName = std::move(other.fullName);           
        phoneNumber = std::move(other.phoneNumber);     
        CCCD = std::move(other.CCCD);                 
        birthday = std::move(other.birthday);        
        creationDate = std::move(other.creationDate);  
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
    
    plainPassword = trimmedPwd;               
    passwordHash = Utils::hashFunc(trimmedPwd);
}

void User::setPlainPassword(const std::string& plainPwd) { 
    plainPassword = plainPwd; 
}

void User::setFullName(const std::string &name){ 
    fullName = Utils::trimmed(name);
}

void User::setPhoneNumber(const std::string &phone) {
        phoneNumber = Utils::trimmed(phone); 
}

void User::setCCCD(const std::string &cccd) { 
    CCCD = Utils::trimmed(cccd); 
}

void User::setBirthday(const std::string &bd) { 
    birthday = Utils::trimmed(bd); 
}

void User::setCreationDate(const Date& date) {
     creationDate = date; 
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
    j["passwordHash"] = passwordHash;
    j["plainPassword"] = plainPassword;
    j["fullName"] = fullName;
    j["phoneNumber"] = phoneNumber;
    j["CCCD"] = CCCD;
    j["birthday"] = birthday;
    j["creationDate"] = creationDate.toJson();
    return j;
}

void User::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("userRole")) userRole = roleFromString(j.at("userRole").get<std::string>());
    if (j.contains("username")) username = j.at("username").get<std::string>();
    if (j.contains("passwordHash")) passwordHash = j.at("passwordHash").get<std::string>();
    if (j.contains("plainPassword")) {
        plainPassword = j.at("plainPassword").get<std::string>();
    } else {
        plainPassword = "";
    }
    if (j.contains("fullName")) fullName = j.at("fullName").get<std::string>();
    if (j.contains("phoneNumber")) phoneNumber = j.at("phoneNumber").get<std::string>();
    if (j.contains("CCCD")) CCCD = j.at("CCCD").get<std::string>();
    if (j.contains("birthday")) birthday = j.at("birthday").get<std::string>();
}
