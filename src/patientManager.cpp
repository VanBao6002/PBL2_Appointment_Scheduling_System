#include "patient.h"
#include "patientManager.h"
#include "utils.h"
#include <algorithm>
#include <unordered_set>

void PatientManager::addPatient(int ID_, const Patient &pat_) {
    if (patientTable.find(ID_) != patientTable.end()) {
        throw std::invalid_argument("Patient ID already exists.");
    }
    patientTable[ID_] = pat_;
    log[ID_] += " Added on " + Utils::getDateTime();
}

void PatientManager::editPatient(int ID_, const Patient &newPatient){
    Utils::validPatientID(patientTable, ID_);
    patientTable[ID_] = newPatient;
    log[ID_] += " Edited on " + Utils::getDateTime();
}

void PatientManager::removePatient(int ID_){
    Utils::validPatientID(patientTable, ID_);
    patientTable.erase(ID_);
    log.erase(ID_);
}

// Getters
const Patient& PatientManager::getPatientByID(int ID_) const{
    Utils::validPatientID(patientTable, ID_);
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
    Utils::validPatientID(patientTable, ID_);
    return log.at(ID_);
}

void PatientManager::loadFromFile(const std::string& filename) {
    Utils::loadFromFile(filename, patientTable);
}

void PatientManager::saveToFile(const std::string& filename) const {
    Utils::saveToFile(filename, patientTable);
}

bool PatientManager::loadFromStream(std::istream& is) {
    return Utils::loadFromStream(is, patientTable);
}

void PatientManager::saveToStream(std::ostream& os) const {
    Utils::saveToStream(os, patientTable);
}