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

        Person(int ID_, const std::string &name_, char gender_, const Date &birthday_);

        virtual ~Person();

        std::string getName() const {return name;}
        char getGender() const {return gender;}
        Date getBirthday() const {return birthday;}
        int getID() const {return ID;}
        virtual std::string getInfo() const = 0;

        void setName(const std::string &name_);
        void setGender(const char gender_);
        void setBirthday(const Date &birthday_); 
        void setID(int ID_); 
};
