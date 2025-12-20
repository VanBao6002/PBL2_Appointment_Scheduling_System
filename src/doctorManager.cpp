#include "doctorManager.h"
#include "IDHandler.h"
#include <qlogging.h>
#include <QDebug>

void DoctorManager::addDoctor(const Doctor &doc_) {
    int ID_ = doc_.getID();
    if (doctorTable.find(ID_) != doctorTable.end()) {
        throw std::invalid_argument("Adding failed. Doctor ID " + std::to_string(doc_.getID()) + " already exists.");
    }
    // Debug output for working schedule
    qDebug() << "[MANAGER DEBUG] Saving doctor ID:" << ID_ << "with working schedule:";
    for (const auto& day : doc_.getWorkingSchedule().schedule) {
        QString dayStr = QString::fromStdString(day.first);
        for (const auto& slot : day.second) {
            QString startStr = QString::fromStdString(slot.first);
            QString endStr = QString::fromStdString(slot.second);
            qDebug() << "  " << dayStr << ":" << startStr << "-" << endStr;
        }
    }
    doctorTable[ID_] = doc_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    saveToFile(Config::DOCTOR_PATH);
}

void DoctorManager::editDoctor(int ID_, const Doctor &newDoctor){
    if (doctorTable.find(ID_) == doctorTable.end()) {
        throw std::invalid_argument("Editing failed. Doctor ID " + std::to_string(newDoctor.getID()) + " not found.");
    }
    // Debug output for working schedule
    qDebug() << "[MANAGER DEBUG] Editing doctor ID:" << ID_ << "with working schedule:";
    for (const auto& day : newDoctor.getWorkingSchedule().schedule) {
        QString dayStr = QString::fromStdString(day.first);
        for (const auto& slot : day.second) {
            QString startStr = QString::fromStdString(slot.first);
            QString endStr = QString::fromStdString(slot.second);
            qDebug() << "  " << dayStr << ":" << startStr << "-" << endStr;
        }
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
    return IDHandler<Doctor>::checkDuplicateID(static_cast<size_t>(doctorID));
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
    IDHandler<Doctor>::resetIDTable(); 
    IDHandler<Person>::resetCCCDTable();

    // check active path, propriate data
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Doctor file is empty or invalid format";
        return;
    }

    // ===== FIX: Parse trực tiếp vào doctorTable, KHÔNG tạo biến tạm =====
    int maxID = 0;
    for (const auto& jDoctor : jArr) {
        try {
            // Tạo doctor object tạm để parse
            Doctor tempDoctor;
            tempDoctor.fromJson(jDoctor);
            int ID = tempDoctor.getID();
            
            if (doctorTable.count(ID)) {
                qWarning() << "[WARNING] Duplicate Doctor ID in file:" << ID << "- Skipping";
                continue;
            }

            // ===== QUAN TRỌNG: Move vào doctorTable, không copy =====
            // Điều này tránh gọi destructor của tempDoctor
            doctorTable[ID] = std::move(tempDoctor);
            
            if (ID > maxID) maxID = ID;
        } catch (const std::exception& e) {
            qWarning() << "[ERROR] Failed to load doctor record:" << e.what();
        }
    }
    
    // ===== Re-register tất cả ID và CCCD từ doctorTable =====
    IDHandler<Doctor>::resetIDTable();
    IDHandler<Person>::resetCCCDTable();

    for (const auto& pair : doctorTable) {
        const Doctor& storedDoc = pair.second;
        
        // Đăng ký lại ID
        try {
            if (!IDHandler<Doctor>::checkDuplicateID(static_cast<size_t>(storedDoc.getID()))) {
                IDHandler<Doctor>::registerID(static_cast<size_t>(storedDoc.getID()));
                qDebug() << "[REGISTER] Doctor ID:" << storedDoc.getID() << "registered successfully";
            }
        } catch (const std::exception& e) {
            qWarning() << "[ERROR] Failed to register Doctor ID" << storedDoc.getID() << ":" << e.what();
        }

        // Đăng ký lại CCCD
        try {
            if (!IDHandler<Person>::checkDuplicateCCCD(storedDoc.getCCCD())) {
                IDHandler<Person>::registerCCCD(storedDoc.getCCCD());
                qDebug() << "[REGISTER] CCCD:" << QString::fromStdString(storedDoc.getCCCD()) << "registered successfully";
            }
        } catch (const std::exception& e) {
            qWarning() << "[ERROR] Failed to register CCCD:" << e.what();
        }
    }
    
    // Set current ID > maxID
    if (maxID >= 0) {
        IDHandler<Doctor>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << doctorTable.size() << "doctors from file";
    
    // ===== VERIFY: In ra tất cả ID đã đăng ký =====
    qDebug() << "[VERIFY] Checking registered Doctor IDs:";
    for (const auto& pair : doctorTable) {
        int id = pair.first;
        bool isRegistered = IDHandler<Doctor>::checkDuplicateID(static_cast<size_t>(id));
        qDebug() << "  Doctor ID" << id << "registered:" << (isRegistered ? "YES" : "NO");
    }
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
