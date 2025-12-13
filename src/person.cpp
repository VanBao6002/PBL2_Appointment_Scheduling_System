#include "person.h"
#include "IDHandler.h"

Person::Person()
    : ID(0),
      name("NGUYEN VAN A"),
      gender('M'),
      birthday(Date()),
      phoneNumber("00000000000"),
      CCCD("XXXXXXXXXXXX"),
      email("foo@gmail.com")
{
}

Person::Person(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_)
    : ID(0)
{
    setName(name_);
    setGender(gender_);
    setBirthday(birthday_);
    setPhoneNumber(phoneNumber_);
    setCCCD(CCCD_);
    IDHandler<Person>::registerCCCD(Utils::trimmed(CCCD_));
    setEmail(email_);
}

Person::~Person(){
    IDHandler<Person>::unregisterCCCD(CCCD);
}

void Person::setName(const std::string &name_) {
    Utils::validName(Utils::trimmed(name_));
    name = Utils::trimmed(name_);
}

void Person::setGender(char gender_) {
    Utils::validGender(gender_);
    gender = gender_;
}

void Person::setBirthday(const std::string &birthday_) {
    Utils::validDate(Date::fromString(Utils::trimmed(birthday_)));
    birthday = Date::fromString(Utils::trimmed(birthday_));
}

void Person::setBirthday(const Date &date_) {
    Utils::validDate(date_);
    birthday = date_;
}

void Person::setPhoneNumber(const std::string &phoneNumber_) {
    Utils::validPhoneNumber(Utils::trimmed(phoneNumber_));
    if(phoneNumber_.size() == 0){
        phoneNumber = "None";
    } else
    phoneNumber = Utils::trimmed(phoneNumber_);
}

void Person::setID(int ID_) {
    ID = ID_;
}

void Person::setCCCD(const std::string &CCCD_) {
    Utils::validCCCD(Utils::trimmed(CCCD_));
    CCCD = Utils::trimmed(CCCD_);
}

void Person::setEmail(const std::string &email_){
    Utils::validEmail(Utils::trimmed(email_));
    email = Utils::trimmed(email_);
}
