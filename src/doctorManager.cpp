#include "doctorManager.h"
#include "IDHandler.h"
#include <qlogging.h>
#include <QDebug>

void DoctorManager::addDoctor(const Doctor &doc_) {
    int ID_ = doc_.getID();
    if (doctorTable.find(ID_) != doctorTable.end()) {
        throw std::invalid_argument("Adding failed. Doctor ID " + std::to_string(doc_.getID()) + " already exists.");
    }
    doctorTable[ID_] = doc_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    saveToFile(Config::DOCTOR_PATH);
}

void DoctorManager::editDoctor(int ID_, const Doctor &newDoctor){
    if (doctorTable.find(ID_) == doctorTable.end()) {
        throw std::invalid_argument("Editing failed. Doctor ID " + std::to_string(newDoctor.getID()) + " not found.");
    }
    doctorTable[ID_] = newDoctor;
    log[ID_] += " Edited on: " + Utils::getDateTime();
    saveToFile(Config::DOCTOR_PATH);
}
                                                                                                                                               
void DoctorManager::removeDoctor(int ID_){
    if (doctorTable.find(ID_) == doctorTable.end()) {
        throw std::invalid_argument("Removing failed. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    doctorTable.erase(ID_);
    log.erase(ID_);
    saveToFile(Config::DOCTOR_PATH);
}

bool DoctorManager::isDoctorExist(int doctorID) const {
    return IDHandler<Doctor>::checkDuplicate(static_cast<size_t>(doctorID));
}

const Doctor& DoctorManager::getDoctorByID(int ID_) const{
    if (doctorTable.find(ID_) == doctorTable.end()) {
        throw std::invalid_argument("Failed getting. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    return doctorTable.at(ID_);
}

const std::unordered_map<int, Doctor>& DoctorManager::getAllDoctors() const{
    return doctorTable;
}

const std::unordered_set<int>& DoctorManager::getPatientsByDoctorID(int ID_) const{
    if (doctorTable.find(ID_) == doctorTable.end()) {
        throw std::invalid_argument("Cannot get patients list. Doctor ID " + std::to_string(ID_) + " not found.");
    }
    return doctorTable.at(ID_).getPatientIDs();
}

std::vector<Doctor> DoctorManager::findDoctorsByName(const std::string& name) const {
    std::vector<Doctor> result;
    for (const auto& pair : doctorTable) {
        if (Utils::toLower(pair.second.getName()) == Utils::toLower(Utils::trimmed(name))) {
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
    if (log.find(ID_) == log.end()) {
        throw std::invalid_argument("Failed getting log. Doctor ID " +  std::to_string(ID_) + " is not found.");
    }
    return log.at(ID_);
}

void DoctorManager::loadFromFile(const std::string& path) {
    // clean data before loading
    doctorTable.clear();
    log.clear();
    IDHandler<Doctor>::reset(); 

    // check active path, propriate data
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Doctor file is empty or invalid format";
        return;
    }

    // start reading and load to memory
    int maxID = 0;
    for (const auto& jDoctor : jArr) {
    try {
        Doctor doctor;
        doctor.fromJson(jDoctor);
        int ID = doctor.getID();
        if (doctorTable.count(ID)) {
            qWarning() << "[WARNING] Duplicate Doctor ID in file:" << ID << "- Skipping";
            continue;
        }
        if (!IDHandler<Doctor>::checkDuplicate(static_cast<size_t>(ID))) {
            IDHandler<Doctor>::registerID(static_cast<size_t>(ID));
        }
        doctorTable[ID] = doctor;
        if (ID > maxID) maxID = ID;
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Failed to load doctor record:" << e.what();
    }
    }
    
    // Set current ID > maxID
    if (maxID >= 0) {
        IDHandler<Doctor>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << doctorTable.size() << "doctors from file";
}

void DoctorManager::saveToFile(const std::string& path){
    try {
        nlohmann::json jArr;
        for (const auto& pair : doctorTable) {
            jArr.push_back(pair.second.toJson());
        }        
        Utils::writeJsonToFile(path, jArr);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save doctors data: " << e.what() << std::endl;
        throw; // rethrow for caller (UI layer) show message
    }
}
