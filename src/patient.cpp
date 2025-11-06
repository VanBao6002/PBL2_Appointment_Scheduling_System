#include "person.h"
#include "utils.h"
#include "patient.h"
#include <iostream>
#include <sstream>

Patient::Patient(): Person(), bloodType(""), nameMother(""), nameFather(""){}

Patient::Patient(const std::string &name_, char gender_, 
                 const Date &birthday_, int ID_, const std::string &bloodType_, 
                 const std::string &nameMother_, const std::string &nameFather_) 
                : Person(ID_, name_, gender_, birthday_), bloodType(bloodType_), 
                nameMother(nameMother_), nameFather(nameFather_){} 

void Patient::setBloodType(const std::string &bloodType_){
    Utils::validBloodType(bloodType_);
    bloodType = bloodType_;
}

void Patient::setNameMother(const std::string &nameMother_){
    nameMother = nameMother_;
}

void Patient::setNameFather(const std::string &nameFather_){
    nameFather = nameFather_;
}

std::string Patient::getInfo() const {
    std::string info = Person::getInfo();
    info += "\nBlood Type: " + bloodType;
    info += "\nMother's Name: " + nameMother;
    info += "\nFather's Name: " + nameFather;
    return info;
}
