#include "patientManager.h"

void PatientManager::addPatient(const Patient &pat_) {
    int ID_ = pat_.getID();
    if (IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Adding failed. Patient ID " + std::to_string(pat_.getID()) + " already exists.");
    }
    patientTable[ID_] = pat_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    IDHandler<Patient>::registerID(ID_);
}

void PatientManager::editPatient(int ID_, const Patient &newPatient){
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Editing failed. Patient ID " + std::to_string(newPatient.getID()) + " not found.");
    }
    patientTable[ID_] = newPatient;
    log[ID_] += " Edited on: " + Utils::getDateTime();
}

void PatientManager::removePatient(int ID_){
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Removing failed. Patient ID " + std::to_string(ID_) + " not found.");
    }
    IDHandler<Patient>::unregisterID(ID_);
    patientTable.erase(ID_);
    log.erase(ID_);
    log[ID_] += " Removed on: " + Utils::getDateTime();
}

// Getters
const Patient& PatientManager::getPatientByID(int ID_) const{
    if (!IDHandler<Patient>::checkDuplicate(ID_)){
        throw std::invalid_argument("Failed getting. Patient ID " + std::to_string(ID_) + " not found.");
    }
    return patientTable.at(ID_);
}

std::vector<Patient> PatientManager::findPatientsByName(const std::string& name) const {
    std::string trimmedName = Utils::trimmed(name);

    std::vector<Patient> result;
    for (const auto& pair : patientTable) {
        if (Utils::toLower(pair.second.getName()) == Utils::toLower(trimmedName)) {
            result.push_back(pair.second);
        }
    }
    if (result.empty()) throw std::invalid_argument("Patient " + name + " is not found.");
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
        throw std::invalid_argument("Failed getting log. Patient ID " +  std::to_string(ID_) + " is not found.");
    }
    return log.at(ID_);
}

void PatientManager::loadFromFile(const std::string& path) {
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    for (const auto& jPatient : jArr) {
        Patient pat;
        pat.fromJson(jPatient);
        addPatient(pat);
    }
}


void PatientManager::saveToFile(const std::string& path){
    nlohmann::json jArr;
    for (const auto& pair : patientTable) {
        jArr.push_back(pair.second.toJson());
    }
    Utils::writeJsonToFile(path, jArr);
}