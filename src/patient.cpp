#include"person.h"
#include"utils.h"
#include"patient.h"
#include<iostream>

Patient::Patient(): Person(), bloodType(""), nameMother(""), nameFather(""){}

Patient::Patient(const std::string &name_, char gender_, 
                 const Date &birthday_, int ID_, const std::string &bloodType_, 
                 const std::string &nameMother_, const std::string &nameFather_) 
                : Person(ID_, name_, gender_, birthday_), bloodType(bloodType_), 
                nameMother(nameMother_), nameFather(nameFather_){} 

Patient::~Patient() {}

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