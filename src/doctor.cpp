#include "doctor.h"
#include "utils.h"
#include "date.h"
#include <algorithm>
#include <unordered_set>
#include <string>

Doctor::Doctor() : Person(), specialization(""), patientIDs(std::unordered_set<int>()), doctorStatus(Status::Available){}

Doctor::Doctor(int ID_, const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, Status doctorStatus_) 
    : Person(ID_, name_, gender_, birthday_), specialization(specialization_), patientIDs(), doctorStatus(doctorStatus_){}

void Doctor::setSpecialization(const std::string &specialization_){
    Utils::validSpecialization(specialization_);
    specialization = specialization_;
}

void Doctor::setStatus(Status doctorStatus_){
    doctorStatus = doctorStatus_;
}
// Add patient ID
void Doctor::addPatientID(int patientID_){
    Utils::validPatientID(patientIDs, patientID_);
    patientIDs.insert(patientID_);
}

// Remove patient ID
void Doctor::removePatientID(int patientID_) {
    Utils::validPatientID(patientIDs, patientID_);
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
    info += (doctorStatus == Doctor::Status::Available) ? "Available" : "Unavailable" "\n";
    info += "Patient IDs: ";
    for (const auto& pid : patientIDs) {
        info += std::to_string(pid) + " ";
    }
    info += "\n";
    return info;
}