#include "person.h"

Person::Person() : ID(0), name("NGUYEN VAN A"), gender('M'), phoneNumber("00000000000"), birthday(Date()) {}

Person::Person(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_) {
    setName(name_);
    setGender(gender_);
    setBirthday(birthday_);
    setPhoneNumber(phoneNumber_);
}

void Person::setName(const std::string &name_){
    Utils::validName(Utils::trimmed(name_));
    name = Utils::trimmed(name_);
}

void Person::setGender(char gender_){
    Utils::validGender(gender_);
    gender = gender_;
}

void Person::setBirthday(const std::string &birthday_){
    Utils::validDate(Date::fromString(Utils::trimmed(birthday_)));
    birthday = Date::fromString(Utils::trimmed(birthday_));
}

void Person::setPhoneNumber(const std::string &phoneNumber_){
    Utils::validPhoneNumber(Utils::trimmed(phoneNumber_));
    phoneNumber = Utils::trimmed(phoneNumber_);
}

void Person::setID(int ID_){
    ID = ID_;   
}

std::string Person::getInfo() const {
    return "ID: " + std::to_string(ID) + 
           "\nName: " + name + 
           "\nGender: " + std::string(1, gender) + 
           "\nBirthday: " + birthday.toString();
}

