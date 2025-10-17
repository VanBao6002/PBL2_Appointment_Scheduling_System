#include "person.h"
#include "utils.h"
#include <cctype>
#include <stdexcept>

//default constructor
Person::Person() : name(""), gender('M'), birthday(Date()), ID(0) {}

//parameterized constructor
Person::Person(const std::string &name_, char gender_, const Date &birthday_, int ID_) : name(name_), gender(gender_), birthday(birthday_), ID(ID_) {
    Utils::validName(name_);
    Utils::validGender(gender_);
    Utils::validDate(birthday_);
    Utils::validID(ID_);
}

//destructor
Person::~Person() {}

//setters
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