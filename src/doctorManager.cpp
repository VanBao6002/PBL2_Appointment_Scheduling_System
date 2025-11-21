#include "doctorManager.h"
#include "IDHandler.h"
#include <qdebug.h>
#include <qlogging.h>

void DoctorManager::addDoctor(const Doctor &doc_) {
    int ID_ = doc_.getID();
    if (doctorTable.find(ID_) != doctorTable.end()) {
        throw std::invalid_argument("Adding failed. Doctor ID " + std::to_string(doc_.getID()) + " already exists.");
    }
    doctorTable[ID_] = doc_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    IDHandler<Doctor>::registerID(static_cast<size_t>(ID_));
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
    IDHandler<Doctor>::unregisterID(static_cast<size_t>(ID_));
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

const std::vector<int>& DoctorManager::getPatientsByDoctorID(int ID_) const{
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
    // ✅ QUAN TRỌNG: Clear dữ liệu cũ trước khi load
    doctorTable.clear();
    log.clear();
    
    // ✅ Reset IDHandler trước khi load (tránh trùng lặp)
    IDHandler<Doctor>::reset(); // Nếu có hàm reset()
    // Hoặc nếu không có reset(), bỏ qua bước này
    
    // Kiểm tra file có tồn tại không
    std::ifstream file(path);
    if (!file.good()) {
        qDebug() << "[WARNING] Doctor file not found:" << QString::fromStdString(path);
        return; // Không có file thì return, không throw error
    }
    file.close();
    
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    
    // Kiểm tra JSON rỗng
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Doctor file is empty or invalid format";
        return;
    }
    
    int maxID = 0;
    
    for (const auto& jDoctor : jArr) {
        Doctor doctor;
        doctor.fromJson(jDoctor);
        
        int ID = doctor.getID();
        
        // ✅ Kiểm tra ID đã tồn tại trong table chưa (tránh duplicate trong file JSON)
        if (doctorTable.find(ID) != doctorTable.end()) {
            qWarning() << "[WARNING] Duplicate Doctor ID in file:" << ID << "- Skipping";
            continue; // Bỏ qua record trùng, không throw error
        }
        
        // ✅ Chỉ register ID nếu chưa tồn tại
        if (!IDHandler<Doctor>::checkDuplicate(static_cast<size_t>(ID))) {
            IDHandler<Doctor>::registerID(static_cast<size_t>(ID));
        }
        
        doctorTable[ID] = doctor;
        
        if (ID > maxID) {
            maxID = ID;
        }
    }
    
    // ✅ Set current ID để ID mới sẽ > maxID
    if (maxID > 0) {
        IDHandler<Doctor>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << doctorTable.size() << "doctors from file";
}

void DoctorManager::saveToFile(const std::string& path){
    nlohmann::json jArr;
    for (const auto& pair : doctorTable) {
        jArr.push_back(pair.second.toJson());
    }
    Utils::writeJsonToFile(path, jArr);
}
