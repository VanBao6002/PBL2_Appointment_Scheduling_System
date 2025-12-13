#include "utils.h"
#include "config.h"
#include "medicalRecordManager.h"
#include "IDHandler.h"
#include "doctor.h"
#include "patient.h"
#include <QDebug>

void MedicalRecordManager::addMedicalRecord(const MedicalRecord &record_) {
    int ID_ = record_.getID();
    if (medicalRecordTable.find(ID_) != medicalRecordTable.end()) {
        throw std::invalid_argument("Adding failed. MedicalRecord ID " + std::to_string(record_.getID()) + " already exists.");
    }
    medicalRecordTable[ID_] = record_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    
    // ✅ Lưu file ngay sau khi thêm
    saveToFile(Config::MEDICAL_RECORD_PATH);
    
    qDebug() << "[INFO] Medical Record" << ID_ << "added and saved successfully";
}

void MedicalRecordManager::editMedicalRecord(int ID_, const MedicalRecord &newMedicalRecord) {
    if (medicalRecordTable.find(ID_) == medicalRecordTable.end()) {
        throw std::invalid_argument("Editing failed. MedicalRecord ID " + std::to_string(newMedicalRecord.getID()) + " not found.");
    }
    medicalRecordTable[ID_] = newMedicalRecord;
    log[ID_] += " Edited on: " + Utils::getDateTime();
    
    // ✅ Lưu file ngay sau khi sửa
    saveToFile(Config::MEDICAL_RECORD_PATH);
}

void MedicalRecordManager::removeMedicalRecord(int ID_) {
    if (medicalRecordTable.find(ID_) == medicalRecordTable.end()) {
        throw std::invalid_argument("Removing failed. MedicalRecord ID " + std::to_string(ID_) + " not found.");
    }
    medicalRecordTable.erase(ID_);
    log.erase(ID_);
    
    // ✅ Lưu file ngay sau khi xóa
    saveToFile(Config::MEDICAL_RECORD_PATH);
}

const MedicalRecord& MedicalRecordManager::getMedicalRecordByID(int ID_) const {
    if (medicalRecordTable.find(ID_) == medicalRecordTable.end()) {
        throw std::invalid_argument("Failed getting. MedicalRecord ID " + std::to_string(ID_) + " not found.");
    }
    return medicalRecordTable.at(ID_);
}

const std::unordered_map<int, MedicalRecord>& MedicalRecordManager::getAllMedicalRecords() const {
    return medicalRecordTable;
}

const std::unordered_map<int, std::string>& MedicalRecordManager::getAllLog() const {
    return log;
}

const std::string& MedicalRecordManager::getIDLog(int ID_) const {
    if (log.find(ID_) == log.end()) {
        throw std::invalid_argument("Failed getting log. MedicalRecord ID " +  std::to_string(ID_) + " not found.");
    }
    return log.at(ID_);
}

void MedicalRecordManager::loadFromFile(const std::string& path) {
    // clean data before loading
    medicalRecordTable.clear();
    log.clear();
    IDHandler<MedicalRecord>::resetIDTable(); 

    // check active path, propriate data
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Records file is empty or invalid format";
        return;
    }

    // start reading and load to memory
    int maxID = 0;
    for (const auto& jmedRecord : jArr) {
    try {
        MedicalRecord medRecord;
        medRecord.fromJson(jmedRecord);
        int ID = medRecord.getID();
        if (medicalRecordTable.count(ID)) {
            qWarning() << "[WARNING] Duplicate medicalRecord ID in file:" << ID << "- Skipping";
            continue;
        }
        if (!IDHandler<MedicalRecord>::checkDuplicateID(static_cast<size_t>(ID))) {
            IDHandler<MedicalRecord>::registerID(static_cast<size_t>(ID));
        }
        medicalRecordTable[ID] = medRecord;
        if (ID > maxID) maxID = ID;
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Failed to load medical record:" << e.what();
    }
    }
    
    // Set current ID > maxID
    if (maxID >= 0) {
        IDHandler<MedicalRecord>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << medicalRecordTable.size() << "medical records from file";
}


void MedicalRecordManager::saveToFile(const std::string& path){
    try {
        nlohmann::json jArr;
        for (const auto& pair : medicalRecordTable) {
            jArr.push_back(pair.second.toJson());
        }        
        Utils::writeJsonToFile(path, jArr);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save medicalRecord data: " << e.what() << std::endl;
        throw; // rethrow for caller (UI layer) show message
    }
}
