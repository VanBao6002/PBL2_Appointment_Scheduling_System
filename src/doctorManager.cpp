#include "doctor.h"
#include "utils.h"
#include "doctorManager.h"
#include <algorithm>
#include <unordered_set>

void DoctorManager::addDoctor(int ID_, const Doctor &doc_) {
    if (doctorTable.find(ID_) != doctorTable.end()) {
        throw std::invalid_argument("Doctor ID already exists.");
    }
    doctorTable[ID_] = doc_;
    log[ID_] = "Added on " + Utils::getDateTime();
}

void DoctorManager::editDoctor(int ID_, const Doctor &newDoctor){
    Utils::checkValidDoctorID(doctorTable, ID_);
    doctorTable[ID_] = newDoctor;
    log[ID_] = "Edited on " + Utils::getDateTime();
}
                                                                                                                                               
void DoctorManager::removeDoctor(int ID_){
    Utils::checkValidDoctorID(doctorTable, ID_);
    doctorTable.erase(ID_);
    log.erase(ID_);
}

Doctor DoctorManager::getDoctorByID(int ID_) const{
    Utils::checkValidDoctorID(doctorTable, ID_);
    return doctorTable.at(ID_);
}

const std::unordered_map<int, Doctor>& DoctorManager::getAllDoctors() const{
    return doctorTable;
}

const std::unordered_set<int>& DoctorManager::getPatientsByDoctorID(int ID_) const{
    return doctorTable.at(ID_).getPatientIDs();
}

std::vector<Doctor> DoctorManager::findDoctorsByName(const std::string& name) const {
    std::vector<Doctor> result;
    for (const auto& pair : doctorTable) {
        if (pair.second.getName() == name) {
            result.push_back(pair.second);
        }
    }
    return result;
}