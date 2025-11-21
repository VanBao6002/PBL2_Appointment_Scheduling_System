#include "patientManager.h"
#include "IDHandler.h"

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
    return IDHandler<Patient>::checkDuplicate(static_cast<size_t>(patientID));
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
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    int maxID = 0;
    
    for (const auto& jPatient : jArr) {
        Patient patient;
        patient.fromJson(jPatient);
        int ID = patient.getID();
        
        if (patientTable.find(ID) != patientTable.end()) {
            throw std::invalid_argument("Loading failed. Patient ID " + std::to_string(ID) + " already exists in table.");
        }
        
        // ✅ Register ID
        try {
            IDHandler<Patient>::registerID(static_cast<size_t>(ID)); 
        } catch (...) {
            // ID đã được register, bỏ qua
        }
        
        patientTable[ID] = patient;
        
        if (ID > maxID) {
            maxID = ID;
        }
    }
    
    if (maxID > 0) {
        IDHandler<Patient>::setCurrentID(static_cast<size_t>(maxID));
    }
}

void PatientManager::saveToFile(const std::string& path){
    try {
        nlohmann::json jArr;
        for (const auto& pair : patientTable) {
            jArr.push_back(pair.second.toJson());
        }
        
        // ✅ Debug: In số lượng bệnh nhân đang lưu
        std::cout << "[DEBUG] Saving " << patientTable.size() << " patients to: " << path << std::endl;
        
        Utils::writeJsonToFile(path, jArr);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save patients: " << e.what() << std::endl;
        throw; // ✅ Ném lại exception để caller biết
    }
}