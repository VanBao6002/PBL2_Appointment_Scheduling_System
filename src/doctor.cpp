#include "doctor.h"
#include "utils.h"
#include "patient.h"
#include "date.h"
#include <limits>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>
#include <string>

Doctor::Doctor() : Person(), specialization(""), patientIDs(std::unordered_map<int, Patient>()), doctorStatus(Status::Available){}

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
void Doctor::addPatientID(const Patient& patient) {
    Utils::validPatientID(patientIDs, patient.getID());
    patientIDs[patient.getID()] = patient;
}

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

void Doctor::serialize(std::ostream& os) const{
        os << getID() << ' ' << getName() << ' ' << getGender() << ' ' 
           << getBirthday().toString() << ' ' << specialization << ' ' 
           << static_cast<int>(doctorStatus) << ' ' << patientIDs.size() << '\n';
        for (const auto& [pid, patient] : patientIDs) {
            patient.serialize(os);
        }
    }

void Doctor::deserialize(std::istream& is){
        int statusInt;
        size_t numPatients;
        std::string birthdayStr;
        int id;
        char gender;
        std::string name;

        is >> id >> name >> gender >> birthdayStr >> specialization >> statusInt >> numPatients;
        is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        setID(id);
        setName(name);
        setGender(gender);
        setBirthday(Date::fromString(birthdayStr));
        doctorStatus = static_cast<Status>(statusInt);

        patientIDs.clear();
        for (size_t i = 0; i < numPatients; ++i) {
            Patient p;
            p.deserialize(is);
            patientIDs[p.getID()] = p;
        }
}
