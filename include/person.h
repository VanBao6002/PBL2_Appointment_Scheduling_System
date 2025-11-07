#pragma once

#include <string>
#include "date.h"
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

        std::string getName() const;
        char getGender() const;
        Date getBirthday() const;
        int getID() const;
        virtual std::string getInfo() const = 0;

        void setName(const std::string &name_);
        void setGender(const char gender_);
        void setBirthday(const Date &birthday_); 
    
    protected:
        void setID(int ID_); 
};
