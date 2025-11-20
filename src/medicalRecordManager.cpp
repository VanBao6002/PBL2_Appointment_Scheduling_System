#include "utils.h"
#include "config.h"
#include "MedicalRecordManager.h"

void MedicalRecordManager::addMedicalRecord(const MedicalRecord &record_) {
    int ID_ = record_.getID();
    if (IDHandler<MedicalRecord>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Adding failed. MedicalRecord ID " + std::to_string(record_.getID()) + " already exists.");
    }
    MedicalRecordTable[ID_] = record_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    IDHandler<MedicalRecord>::registerID(ID_);
}

void MedicalRecordManager::editMedicalRecord(int ID_, const MedicalRecord &newMedicalRecord) {
    if (!IDHandler<MedicalRecord>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Editing failed. MedicalRecord ID " + std::to_string(newMedicalRecord.getID()) + " not found.");
    }
    MedicalRecordTable[ID_] = newMedicalRecord;
    log[ID_] += " Edited on: " + Utils::getDateTime();
    appendLogToFile("Edited MedicalRecord ID " + std::to_string(ID_) + " on " + Utils::getDateTime());
}

void MedicalRecordManager::removeMedicalRecord(int ID_) {
    if (!IDHandler<MedicalRecord>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Removing failed. MedicalRecord ID " + std::to_string(ID_) + " not found.");
    }
    IDHandler<MedicalRecord>::unregisterID(ID_);
    MedicalRecordTable.erase(ID_);
    log.erase(ID_);
    appendLogToFile("Removed MedicalRecord ID " + std::to_string(ID_) + " on " + Utils::getDateTime());
}
void MedicalRecordManager::appendLogToFile(const std::string& entry) {
    std::ofstream outFile(logFilePath, std::ios::app);
    if (outFile.is_open()) {
        outFile << entry << '\n';
    } else {
        std::cerr << "Error: Could not open log file '" << logFilePath << "' for writing.\n";
    }
}

const MedicalRecord& MedicalRecordManager::getMedicalRecordByID(int ID_) const {
    if (!IDHandler<MedicalRecord>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Failed getting. MedicalRecord ID " + std::to_string(ID_) + " not found.");
    }
    return MedicalRecordTable.at(ID_);
}

const std::unordered_map<int, MedicalRecord>& MedicalRecordManager::getAllMedicalRecords() const {
    return MedicalRecordTable;
}

const std::unordered_map<int, std::string>& MedicalRecordManager::getAllLog() const {
    return log;
}

const std::string& MedicalRecordManager::getIDLog(int ID_) const {
    if (!IDHandler<MedicalRecord>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Failed getting log. MedicalRecord ID " +  std::to_string(ID_) + " not found.");
    }
    return log.at(ID_);
}

void MedicalRecordManager::loadFromFile(const std::string& path) {
    nlohmann::json jArr = Utils::readJsonFromFile(path);

    for (const auto& jMedicalRecord : jArr) {
        MedicalRecord record_;
        record_.fromJson(jMedicalRecord);
        // check relationship between doctor - patient before loading to MedicalRecord manager.
        if (!IDHandler<Doctor>::checkDuplicate(record_.getDoctorID())){
            throw std::invalid_argument("Failed loading from: " + path + ". MedicalRecord ID: " + std::to_string(record_.getID()) + ". Doctor ID " + std::to_string(record_.getDoctorID()) + " not found.");
        }

        if (!IDHandler<Patient>::checkDuplicate(record_.getPatientID())){
            throw std::invalid_argument("Failed loading from: " + path + ". MedicalRecord ID: " + std::to_string(record_.getID()) + ". Patient ID " + std::to_string(record_.getPatientID()) + " not found.");
        }
        addMedicalRecord(record_);
    }
}


void MedicalRecordManager::saveToFile(const std::string& path){
    nlohmann::json jArr;
    for (const auto& pair : MedicalRecordTable) {
        jArr.push_back(pair.second.toJson());   
    }
    Utils::writeJsonToFile(path, jArr);
}