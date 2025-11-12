#include "doctor.h"
#include "utils.h"
#include "doctorManager.h"
#include "IDHandler.h"
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

void DoctorManager::addDoctor(const Doctor &doc_) {
    int ID_ = doc_.getID();
    if (IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Adding doctor failed. Doctor ID " + std::to_string(doc_.getID()) + " already exists.");
    }
    doctorTable[ID_] = doc_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    IDHandler<Doctor>::registerID(ID_);
}

void DoctorManager::editDoctor(int ID_, const Doctor &newDoctor){
    if (!IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Editing doctor failed. Doctor ID " + std::to_string(newDoctor.getID()) + " not found.");
    }
    doctorTable[ID_] = newDoctor;
    log[ID_] += " Edited on: " + Utils::getDateTime();
}
                                                                                                                                               
void DoctorManager::removeDoctor(int ID_){
    if (!IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Removing doctor failed. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    doctorTable.erase(ID_);
    log.erase(ID_);
}

void DoctorManager::changeStatus(int ID_, Doctor::Status status_){
    if (!IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Cannot change status. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    doctorTable[ID_].setStatus(status_);
    log[ID_] += " Status changed on: " + Utils::getDateTime();
}

const Doctor& DoctorManager::getDoctorByID(int ID_) const{
    if (!IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Failed getting doctor. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    return doctorTable.at(ID_);
}

const std::unordered_map<int, Doctor>& DoctorManager::getAllDoctors() const{
    return doctorTable;
}

const std::unordered_map<int, Patient>& DoctorManager::getPatientsByDoctorID(int ID_) const{
    if (!IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Cannot get patients list. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    return doctorTable.at(ID_).getPatientIDs();
}

std::vector<Doctor> DoctorManager::findDoctorsByName(const std::string& name) const {
    std::vector<Doctor> result;
    for (const auto& pair : doctorTable) {
        if (pair.second.getName() == name) {
            result.push_back(pair.second);
        }
    }
    if (result.empty()) throw std::invalid_argument("Doctor " + name + " is not found.");
    return result;
}

const std::unordered_map<int, std::string>& DoctorManager::getAllLog() const {
    return log;
}

const std::string& DoctorManager::getIDLog(int ID_) const {
    if (!IDHandler<Doctor>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Failed getting log. Doctor ID " +  std::to_string(ID_) + " is not found.");
    }
    return log.at(ID_);
}
