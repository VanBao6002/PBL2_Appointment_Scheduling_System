#include "patientManager.h"
#include "IDHandler.h"
#include <QDebug>

void PatientManager::addPatient(const Patient &pat_) {
    int ID_ = pat_.getID();
    
    // ✅ Kiểm tra ID có tồn tại trong table không
    if (patientTable.find(ID_) != patientTable.end()){
        throw std::invalid_argument("Adding failed. Patient ID " + std::to_string(pat_.getID()) + " already exists.");
    }
    
    // ✅ KHÔNG copy lại, chỉ lưu trực tiếp reference
    // Nếu phải lưu, hãy sử dụng move để tránh copy constructor
    patientTable[ID_] = pat_;
    
    log[ID_] += " Added on: " + Utils::getDateTime();
    saveToFile(Config::PATIENT_PATH);
}

void PatientManager::editPatient(int ID_, const Patient &newPatient){
    if (patientTable.find(ID_) == patientTable.end()){
        throw std::invalid_argument("Editing failed. Patient ID " + std::to_string(newPatient.getID()) + " not found.");
    }
    patientTable[ID_] = newPatient;
    log[ID_] += " Edited on: " + Utils::getDateTime();
    saveToFile(Config::PATIENT_PATH);
}

void PatientManager::removePatient(int ID_){
    if (patientTable.find(ID_) == patientTable.end()){
        throw std::invalid_argument("Removing failed. Patient ID " + std::to_string(ID_) + " not found.");
    }
    patientTable.erase(ID_);
    log.erase(ID_);
    try {
        IDHandler<Patient>::unregisterID(static_cast<size_t>(ID_));
    } catch (...) {}
    saveToFile(Config::PATIENT_PATH);
}

bool PatientManager::isPatientExist(int patientID) const {
    return IDHandler<Patient>::checkDuplicateID(static_cast<size_t>(patientID));
}

// Getters
const Patient& PatientManager::getPatientByID(int ID_) const{
    if (patientTable.find(ID_) == patientTable.end()){
        throw std::invalid_argument("Failed getting. Patient ID " + std::to_string(ID_) + " not found.");
    }
    return patientTable.at(ID_);
}

std::vector<Patient> PatientManager::findPatientsByName(const std::string& name) const {
    std::vector<Patient> result;
    for (const auto& pair : patientTable) {
        if (Utils::toLower(pair.second.getName()) == Utils::toLower(Utils::trimmed(name))) {
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
    if (log.find(ID_) == log.end()){
        throw std::invalid_argument("Failed getting log. Patient ID " +  std::to_string(ID_) + " is not found.");
    }
    return log.at(ID_);
}

void PatientManager::loadFromFile(const std::string& path) {
    // clean data before loading
    patientTable.clear();
    log.clear();
    IDHandler<Patient>::resetIDTable(); 

    // check active path, propriate data
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Patient file is empty or invalid format";
        return;
    }

    // start reading and load to memory
    int maxID = 0;
    for (const auto& jPatient : jArr) {
    try {
        Patient pat;
        pat.fromJson(jPatient);
        int ID = pat.getID();
        if (patientTable.count(ID)) {
            qWarning() << "[WARNING] Duplicate patient ID in file:" << ID << "- Skipping";
            continue;
        }
        if (!IDHandler<Patient>::checkDuplicateID(static_cast<size_t>(ID))) {
            IDHandler<Patient>::registerID(static_cast<size_t>(ID));
        }
        if (!IDHandler<Person>::checkDuplicateCCCD(pat.getCCCD())) {
            IDHandler<Person>::registerCCCD(pat.getCCCD());
        }
        patientTable[ID] = pat;
        if (ID > maxID) maxID = ID;
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Failed to load patient record:" << e.what();
    }
    }
    
    // Set current ID > maxID
    if (maxID >= 0) {
        IDHandler<Patient>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << patientTable.size() << "patients from file";
}

void PatientManager::saveToFile(const std::string& path){
    try {
        nlohmann::json jArr;
        for (const auto& pair : patientTable) {
            jArr.push_back(pair.second.toJson());
        }        
        Utils::writeJsonToFile(path, jArr);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save patients data: " << e.what() << std::endl;
        throw; // rethrow for caller (UI layer) show message
    }
}