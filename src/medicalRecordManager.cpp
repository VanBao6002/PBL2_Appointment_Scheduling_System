#include "utils.h"
#include "config.h"
#include "medicalRecordManager.h"
#include "IDHandler.h"
#include "doctor.h"
#include "patient.h"

void MedicalRecordManager::addMedicalRecord(const MedicalRecord &record_) {
    int ID_ = record_.getID();
    if (medicalRecordTable.find(ID_) != medicalRecordTable.end()) {
        throw std::invalid_argument("Adding failed. MedicalRecord ID " + std::to_string(record_.getID()) + " already exists.");
    }
    medicalRecordTable[ID_] = record_;
    log[ID_] += " Added on: " + Utils::getDateTime();
}

void MedicalRecordManager::editMedicalRecord(int ID_, const MedicalRecord &newMedicalRecord) {
    if (medicalRecordTable.find(ID_) == medicalRecordTable.end()) {
        throw std::invalid_argument("Editing failed. MedicalRecord ID " + std::to_string(newMedicalRecord.getID()) + " not found.");
    }
    medicalRecordTable[ID_] = newMedicalRecord;
    log[ID_] += " Edited on: " + Utils::getDateTime();
}

void MedicalRecordManager::removeMedicalRecord(int ID_) {
    if (medicalRecordTable.find(ID_) == medicalRecordTable.end()) {
        throw std::invalid_argument("Removing failed. MedicalRecord ID " + std::to_string(ID_) + " not found.");
    }
    medicalRecordTable.erase(ID_);
    log.erase(ID_);
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
    nlohmann::json jArr = Utils::readJsonFromFile(path);

    for (const auto& jMedicalRecord : jArr) {
        MedicalRecord record_;
        record_.fromJson(jMedicalRecord);
        // kiểm tra ràng buộc doctoc , patient
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
    for (const auto& pair : medicalRecordTable) {
        jArr.push_back(pair.second.toJson());   
    }
    Utils::writeJsonToFile(path, jArr);
}