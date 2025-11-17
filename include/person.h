#pragma once

#include <string>
#include <cctype>
#include <stdexcept>

#include "utils.h"
#include "date.h"
#include "json.hpp"
#include "IDHandler.h"

class Person {
    protected:
        int ID;
        std::string name;
        char gender;
        Date birthday;
    public:
        Person();

        Person(const std::string &name_, char gender_, const Date &birthday_);

        virtual ~Person() = default;

        //getter
        std::string getName() const { return name; }
        char getGender() const { return gender; }
        Date getBirthday() const { return birthday; }
        int getID() const { return ID; }
        virtual std::string getInfo() const = 0;

        // setter
        void setName(const std::string &name_);
        void setGender(const char gender_);
        void setBirthday(const Date &birthday_); 

        // converter
        virtual nlohmann::json toJson() const = 0;
        virtual void fromJson(const nlohmann::json &j) = 0;

    protected:
        void setID(int ID_); 
};
