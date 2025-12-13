#pragma once

#include <string>
#include <cctype>
#include <stdexcept>

#include "utils.h"
#include "date.h"

class Person {
protected:
    int ID;
    std::string name;
    char gender;
    Date birthday;
    std::string phoneNumber;
    std::string CCCD;
    std::string email;

public:
    Person();
    Person(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_);

    virtual ~Person();

    std::string getName() const { return name; }
    char getGender() const { return gender; }
    Date getBirthday() const { return birthday; }
    std::string getPhoneNumber() const { return phoneNumber; }
    int getID() const { return ID; }
    virtual std::string getInfo() const = 0;
    std::string getCCCD() const { return CCCD; }
    std::string getEmail() const { return email; }


    void setName(const std::string &name_);
    void setGender(char gender_); 
    void setBirthday(const std::string &birthday_);
    void setBirthday(const Date &date_);
    void setPhoneNumber(const std::string &phoneNumber_);
    void setCCCD(const std::string &CCCD_);
    void setEmail(const std::string &email_);
    
    // converter
    virtual nlohmann::json toJson() const = 0;
    virtual void fromJson(const nlohmann::json &j) = 0;

protected:
    void setID(int ID_);
};
