#include "person.h"
#include "utils.h"
#include <cctype>
#include <stdexcept>

Person::Person() :  ID(0), name(""), gender('M'), birthday(Date()) {}

Person::Person(int ID_, const std::string &name_, char gender_, const Date &birthday_) : ID(ID_), name(name_), gender(gender_), birthday(birthday_) {
    Utils::validName(name_);
    Utils::validGender(gender_);
    Utils::validDate(birthday_);
    Utils::validID(ID_);
}

Person::~Person() {}

void Person::setName(const std::string &name_){
    Utils::validName(name_);
    name = name_;
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
    Utils::validID(ID_);
    ID = ID_;   
}

std::string Person::getInfo() const {
    return "ID: " + std::to_string(ID) + 
           "\nName: " + name + 
           "\nGender: " + std::string(1, gender) + 
           "\nBirthday: " + birthday.toString();
}

std::string Person::getName() const {
    return name;
}

char Person::getGender() const {
    return gender;
}

Date Person::getBirthday() const {
    return birthday;
}

int Person::getID() const {
    return ID;
}
