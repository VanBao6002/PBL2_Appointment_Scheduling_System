#ifndef PERSON_H
#define PERSON_H

#include <string>
#include "date.h"
class Person {
    protected:
        std::string name;
        char gender;
        Date birthday;
        int ID;
    public:
        //default constructor
        Person();

        //parameterized constructor
        Person(const std::string &name_, char gender_, const Date &birthday_, int ID_);

        //virtual destructor
        virtual ~Person();

        //pure virtual func, must be overridden by derived classes, person class now cannot be instantiated
        virtual void displayInfo() const = 0;

        //getters
        std::string getName() const {return name;}
        char getGender() const {return gender;}
        Date getBirthday() const {return birthday;}
        int getID() const {return ID;}

        //setters
        void setName(const std::string &name_);
        void setGender(const char gender_);
        void setBirthday(const Date &birthday_); 
        void setID(int ID_); 
};

#endif
