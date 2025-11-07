#include "patient.h"
#include "IDHandler.h"
#include "utils.h"
#include <iostream>

Patient::Patient(): Person(), bloodType(""), nameMother(""), nameFather(""){
    int ID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(ID);
    IDHandler<Patient>::registerID(ID);
}

Patient::Patient(const std::string &name_, char gender_, const Date &birthday_, const std::string &bloodType_, const std::string &nameMother_, const std::string &nameFather_) : Person(name_, gender_, birthday_), bloodType (bloodType_), nameMother(nameMother_), nameFather(nameFather_){
    int ID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(ID);
    IDHandler<Patient>::registerID(ID);
} 

Patient::~Patient() {
    IDHandler<Patient>::unregisterID(ID);
}
Patient::Patient() : Person(), bloodType(""), nameMother(""), nameFather("") {}

Patient::Patient(const std::string &name_, char gender_,
                 const Date &birthday_, int ID_,
                 const std::string &bloodType_,
                 const std::string &nameMother_,
                 const std::string &nameFather_)
    : Person(ID_, name_, gender_, birthday_),
      bloodType(bloodType_),
      nameMother(nameMother_),
      nameFather(nameFather_) {}

void Patient::setBloodType(const std::string &bloodType_) {
    Utils::validBloodType(bloodType_);
    bloodType = bloodType_;
}

void Patient::setNameMother(const std::string &nameMother_) { nameMother = nameMother_; }
void Patient::setNameFather(const std::string &nameFather_) { nameFather = nameFather_; }

std::string Patient::getInfo() const {
    std::string info = Person::getInfo();
    info += "\nBlood Type: " + bloodType;
    info += "\nMother's Name: " + nameMother;
    info += "\nFather's Name: " + nameFather;
    return info;
}

// ---------------- JSON -----------------
nlohmann::json Patient::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = {
        {"day", birthday.getDay()},
        {"month", birthday.getMonth()},
        {"year", birthday.getYear()}
    };
    j["bloodType"] = bloodType;
    j["nameMother"] = nameMother;
    j["nameFather"] = nameFather;
    return j;
}

void Patient::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("name")) name = j.at("name").get<std::string>();
    if (j.contains("gender")) {
        std::string g = j.at("gender").get<std::string>();
        if (!g.empty()) gender = g[0];
    }
    if (j.contains("birthday")) {
        auto bd = j.at("birthday");
        int d = bd.value("day", 1);
        int m = bd.value("month", 1);
        int y = bd.value("year", 2000);
        birthday = Date(d, m, y);
    }
    if (j.contains("bloodType")) bloodType = j.at("bloodType").get<std::string>();
    if (j.contains("nameMother")) nameMother = j.at("nameMother").get<std::string>();
    if (j.contains("nameFather")) nameFather = j.at("nameFather").get<std::string>();
}
