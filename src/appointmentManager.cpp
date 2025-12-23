#include "appointmentManager.h"
#include <set>
#include <QDebug>

void AppointmentManager::addAppointment(const Appointment &apt_) {
    qDebug() << "[DEBUG][addAppointment] Entered function.";
    int ID_ = apt_.getID();

    if (appointmentTable.find(ID_) != appointmentTable.end()) {
        throw std::invalid_argument("Adding failed. Appointment ID " + std::to_string(apt_.getID()) + " already exists.");
    }

    if (!IDHandler<Doctor>::checkDuplicateID(apt_.getDoctorID())) {
        throw std::invalid_argument("Lỗi, ID Bác sĩ không tồn tại!");
    }
    if (!IDHandler<Patient>::checkDuplicateID(apt_.getPatientID())) {
        throw std::invalid_argument("Lỗi, ID Bệnh nhân không tồn tại!");
    }
    
    if (checkDuplicateAppointment(apt_)){
        throw std::invalid_argument("Lỗi, Lịch hẹn bị trùng lặp!");
    }

    appointmentTable[ID_] = apt_;
    log[ID_] += " Added on: " + Utils::getDateTime();
}

void AppointmentManager::editAppointment(int ID_, const Appointment &newAppointment) {
    if (appointmentTable.find(ID_) == appointmentTable.end()) {
        throw std::invalid_argument("Editing failed. Appointment ID " + std::to_string(newAppointment.getID()) + " not found.");
    }
    
    if (!IDHandler<Doctor>::checkDuplicateID(newAppointment.getDoctorID())) {
        throw std::invalid_argument("Lỗi, ID Bác sĩ không tồn tại!");
    }
    if (!IDHandler<Patient>::checkDuplicateID(newAppointment.getPatientID())) {
        throw std::invalid_argument("Lỗi, ID Bệnh nhân không tồn tại!");
    }
    
    if (checkDuplicateAppointment(newAppointment)){
        throw std::invalid_argument("Lỗi, Lịch hẹn bị trùng lặp!");
    }

    appointmentTable[ID_] = newAppointment;
    log[ID_] += " Edited on: " + Utils::getDateTime();
}

void AppointmentManager::removeAppointment(int ID_) {
    if (appointmentTable.find(ID_) == appointmentTable.end()) {
        throw std::invalid_argument("Removing failed. Appointment ID " + std::to_string(ID_) + " not found.");
    }
    appointmentTable.erase(ID_);
    log.erase(ID_);
}

const Appointment& AppointmentManager::getAppointmentByID(int ID_) const {
    if (appointmentTable.find(ID_) == appointmentTable.end()) {
        throw std::invalid_argument("Failed getting. Appointment ID " + std::to_string(ID_) + " not found.");
    }
    return appointmentTable.at(ID_);
}

const std::unordered_map<int, Appointment>& AppointmentManager::getAllAppointmentsMap() const {
    return appointmentTable;
}

std::vector<Appointment> AppointmentManager::getAllAppointmentsAsVector() const {
    std::vector<Appointment> result;
    result.reserve(appointmentTable.size());
    for (const auto& pair : appointmentTable) {
        result.push_back(pair.second);
    }
    return result;
}

const std::unordered_map<int, std::string>& AppointmentManager::getAllLog() const {
    return log;
}

const std::string& AppointmentManager::getIDLog(int ID_) const {
    if (log.find(ID_) == log.end()) {
        throw std::invalid_argument("Failed getting log. Appointment ID " +  std::to_string(ID_) + " not found.");
    }
    return log.at(ID_);
}

const std::vector<std::pair<std::string, std::string>>& AppointmentManager::getBookedSlotsForDoctorDate(int selectedDoctorID, const std::string& selectedDate) {
    static std::vector<std::pair<std::string, std::string>> bookedSlots;
    bookedSlots.clear();
    for (const auto& pair : appointmentTable) {
        const Appointment& apt = pair.second;
        if (apt.getDoctorID() == selectedDoctorID && apt.getDate().toString() == selectedDate) {
            bookedSlots.emplace_back(apt.getStartTime(), apt.getEndTime());
        }
    }
    return bookedSlots;
}

bool AppointmentManager::checkDuplicateAppointment(const Appointment &apt_){
    for (const auto& pair : appointmentTable) {
        const Appointment& existing = pair.second;
        
        if (existing.getDoctorID() == apt_.getDoctorID() &&
            existing.getDate() == apt_.getDate()) {
            
            std::string existingStart = existing.getStartTime();
            std::string existingEnd = existing.getEndTime();
            std::string newStart = apt_.getStartTime();
            std::string newEnd = apt_.getEndTime();
            
            if (!(existingEnd <= newStart || existingStart >= newEnd)) {
                return true;
            }
        }
        
        if (existing.getPatientID() == apt_.getPatientID() &&
            existing.getDate() == apt_.getDate()) {
            
            std::string existingStart = existing.getStartTime();
            std::string existingEnd = existing.getEndTime();
            std::string newStart = apt_.getStartTime();
            std::string newEnd = apt_.getEndTime();
            
            if (!(existingEnd <= newStart || existingStart >= newEnd)) {
                return true;
            }
        }
    }
    return false;
}

void AppointmentManager::removeDuplicates() {
    std::set<std::string> seen;
    std::vector<int> toRemove;
    
    for (const auto& pair : appointmentTable) {
        const Appointment& apt = pair.second;
        std::string key = std::to_string(apt.getPatientID()) + "_" +
                         std::to_string(apt.getDoctorID()) + "_" +
                         apt.getDate().toString() + "_" +
                         apt.getStartTime();
        
        if (seen.count(key)) {
            toRemove.push_back(apt.getID());
            qDebug() << "[DUPLICATE] Found duplicate appointment ID:" << apt.getID();
        } else {
            seen.insert(key);
        }
    }
    
    for (int id : toRemove) {
        appointmentTable.erase(id);
        log.erase(id);
        qDebug() << "[REMOVED] Duplicate appointment ID:" << id;
    }
    
    if (!toRemove.empty()) {
        saveToFile(Config::APPOINTMENT_PATH);
        qDebug() << "[CLEANUP] Removed" << toRemove.size() << "duplicate appointments";
    }
}

void AppointmentManager::loadFromFile(const std::string& path) {
    appointmentTable.clear();
    log.clear();
    IDHandler<Appointment>::resetIDTable(); 

    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Appointment file is empty or invalid format";
        return;
    }

    int maxID = 0;
    for (const auto& jApt : jArr) {
    try {
        Appointment apt;
        apt.fromJson(jApt);
        int ID = apt.getID();
        if (appointmentTable.count(ID)) {
            qWarning() << "[WARNING] Duplicate appointment ID in file:" << ID << "- Skipping";
            continue;
        }
        if (!IDHandler<Appointment>::checkDuplicateID(static_cast<size_t>(ID))) {
            IDHandler<Appointment>::registerID(static_cast<size_t>(ID));
        }
        if (!IDHandler<Doctor>::checkDuplicateID(static_cast<size_t>(apt.getDoctorID()))) {
            throw std::invalid_argument("Doctor ID: " + std::to_string(apt.getDoctorID()) + " not found in appointment:" + std::to_string(ID));
        }
        if (!IDHandler<Patient>::checkDuplicateID(static_cast<size_t>(apt.getPatientID()))) {
            throw std::invalid_argument("Patient ID: " + std::to_string(apt.getPatientID()) + " not found in appointment:" + std::to_string(ID));
        }
        appointmentTable[ID] = apt;

        if (ID > maxID) maxID = ID;
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Failed to load appointment record:" << e.what();
    }
    }
    
    if (maxID >= 0) {
        IDHandler<Appointment>::setCurrentID(static_cast<size_t>(maxID));
    }
    
    qDebug() << "[INFO] Loaded" << appointmentTable.size() << "appointments from file";
}

void AppointmentManager::saveToFile(const std::string& path){
    try {
        nlohmann::json jArr;
        for (const auto& pair : appointmentTable) {
            jArr.push_back(pair.second.toJson());
        }        
        Utils::writeJsonToFile(path, jArr);
        
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Failed to save appointments data: " << e.what() << std::endl;
        throw;
    }
}
