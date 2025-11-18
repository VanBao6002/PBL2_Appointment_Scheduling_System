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

    public:
        Person();

        Person(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_);

        virtual ~Person() = default;

        //getter
        std::string getName() const { return name; }
        char getGender() const { return gender; }
        Date getBirthday() const { return birthday; }
        std::string getPhoneNumber() const {return phoneNumber;}
        int getID() const { return ID; }
        virtual std::string getInfo() const = 0;

        // setter
        void setName(const std::string &name_);
        void setGender(const char gender_);
        void setBirthday(const std::string &birthday_); 
        void setPhoneNumber(const std::string &phonenumber_);

        // converter
        virtual nlohmann::json toJson() const = 0;
        virtual void fromJson(const nlohmann::json &j) = 0;

    protected:
        void setID(int ID_); 
};
