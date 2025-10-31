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

bool Patient::loadFromStream(std::istream& is) {
    std::string line;
    if (!std::getline(is, line)) return false;
    
    std::istringstream iss(line);
    int id;
    std::string name;
    char gender;
    std::string dateStr;
    
    // Read base class (Person) data
    if (!(iss >> id >> name >> gender >> dateStr)) return false;
    
    Date birthday;
    std::istringstream dateStream(dateStr);
    int day, month, year;
    char delimiter;
    dateStream >> day >> delimiter >> month >> delimiter >> year;
    birthday = Date(day, month, year);
    
    // Read Patient specific data
    std::string bloodType, motherName, fatherName;
    if (!(iss >> bloodType >> motherName >> fatherName)) return false;
    
    // Set all the values
    setID(id);
    setName(name);
    setGender(gender);
    setBirthday(birthday);
    setBloodType(bloodType);
    setNameMother(motherName);
    setNameFather(fatherName);
    
    return true;
}

void Patient::saveToStream(std::ostream& os) const {
    // Format: ID Name Gender Birthday BloodType MotherName FatherName
    os << getID() << " "
       << getName() << " "
       << getGender() << " "
       << getBirthday().toString() << " "
       << bloodType << " "
       << nameMother << " "
       << nameFather;
}