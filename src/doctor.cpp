#include "doctor.h"
#include "utils.h"
#include "patient.h"
#include "date.h"
#include "IDHandler.h"
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <string>

Doctor::Doctor() : Person(), specialization(""), patientIDs(), doctorStatus(Status::Available){
    ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
}
Doctor::Doctor(const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, Status doctorStatus_) : Person(name_, gender_, birthday_), specialization(specialization_), patientIDs(), doctorStatus(doctorStatus_) {
    int ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
} 

void Doctor::setSpecialization(const std::string &specialization_){
    Utils::validSpecialization(specialization_);
    specialization = specialization_;
}

void Doctor::setStatus(Status doctorStatus_){
    doctorStatus = doctorStatus_;
}
// Add patient ID
void Doctor::addPatientID(const Patient& patient) {
    patientIDs[patient.getID()] = patient;
}

void Doctor::removePatientID(int patientID_) {
    if (!IDHandler<Patient>::checkDuplicate(patientID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    patientIDs.erase(patientID_);
}


std::string Doctor::getInfo() const{
    std::string info = "Doctor Info:\n";
    info += "Name: " + getName() + "\n";
    info += "Gender: " + std::string(1, getGender()) + "\n";
    info += "Birthday: " + getBirthday().toString() + "\n"; 
    info += "ID: " + std::to_string(getID()) + "\n";
    info += "Specialization: " + specialization + "\n";
    info += "Status: ";
    std::string statusStr = (doctorStatus == Doctor::Status::Available) ? "Available" : "Unavailable";
    info += (doctorStatus == Doctor::Status::Available) ? "Available\n" : "Unavailable\n";
    info += "Patient IDs: ";
    for (const auto& [pid, patient] : patientIDs) {
        info += std::to_string(pid) + " (" + patient.getName() + ") ";
    }
    info += "\n";
    return info;
}

std::string getSpecialization(const std::string &specialization_){
    return specialization_;
}

Doctor::Status getStatus(const Doctor::Status &doctorStatus_){
    return doctorStatus_;
}

const std::unordered_map<int, Patient> getPatientIDs(const std::unordered_map<int, Patient> &patientIDs_){
    return patientIDs_;
}

Doctor::Status Doctor::statusFromString(const std::string& str){
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "available") return Doctor::Status::Available;
    if (lowerStr == "unavailable") return Doctor::Status::Unavailable;
    throw std::invalid_argument("Unknown status: " + str);
}

nlohmann::json Doctor::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = {
        {"day", birthday.getDay()},
        {"month", birthday.getMonth()},
        {"year", birthday.getYear()}
    };
    j["specialization"] = specialization;
    j["doctorStatus"] = doctorStatus;
    return j;
}

void Doctor::fromJson(const nlohmann::json &j) {
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
    if (j.contains("specialization")) specialization = j.at("specialization").get<std::string>();
    if (j.contains("doctorStatus")) doctorStatus = statusFromString(j.at("doctorStatus").get<std::string>());
}