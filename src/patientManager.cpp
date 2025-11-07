#include "patient.h"
#include "patientManager.h"
#include "utils.h"
#include "IDHandler.h"
#include <algorithm>
#include <unordered_set>

void PatientManager::addPatient(const Patient &pat_) {
    int ID_ = pat_.getID();
    if (IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Patient ID already exists.");
    }
    patientTable[ID_] = pat_;
    log[ID_] += " Added on " + Utils::getDateTime();
}

void PatientManager::editPatient(int ID_, const Patient &newPatient){
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    patientTable[ID_] = newPatient;
    log[ID_] += " Edited on " + Utils::getDateTime();
}

void PatientManager::removePatient(int ID_){
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    patientTable.erase(ID_);
    log.erase(ID_);
    IDHandler<Patient>::unregisterID(ID_);
}

// Getters
const Patient& PatientManager::getPatientByID(int ID_) const{
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    return patientTable.at(ID_);
}

std::vector<Patient> PatientManager::findPatientsByName(const std::string& name) const {
    std::vector<Patient> result;
    for (const auto& pair : patientTable) {
        if (pair.second.getName() == name) {
            result.push_back(pair.second);
        }
    }
    return result;
}

const std::unordered_map<int, Patient>& PatientManager::getAllPatientsTable() const{
    return patientTable;
}

const std::unordered_map<int, std::string>& PatientManager::getAllLog() const {
    return log;
}

const std::string& PatientManager::getIDLog(int ID_) const {
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    return log.at(ID_);
}
