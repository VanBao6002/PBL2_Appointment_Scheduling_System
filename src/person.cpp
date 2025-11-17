#include "person.h"

Person::Person() : ID(0), name("NGUYEN VAN A"), gender('M'), birthday(Date()) {}

Person::Person(const std::string &name_, char gender_, const Date &birthday_) {
    std::string trimmedName = Utils::trimmed(name_);

    setName(trimmedName);
    setGender(gender_);
    setBirthday(birthday_);
}

void Person::setName(const std::string &name_){
    std::string trimmedName = Utils::trimmed(name_);
    Utils::validName(trimmedName);
    name = trimmedName;
}

void Person::setGender(char gender_){
    Utils::validGender(gender_);
    gender = gender_;
}

void Person::setBirthday(const Date &birthday_){
    Utils::validDate(birthday_);
    birthday = birthday_;
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

