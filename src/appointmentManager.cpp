#include "appointmentManager.h"
#include <QDebug>

void AppointmentManager::addAppointment(const Appointment &apt_) {
    int ID_ = apt_.getID();
    if (appointmentTable.find(ID_) != appointmentTable.end()) {
        throw std::invalid_argument("Adding failed. Appointment ID " + std::to_string(apt_.getID()) + " already exists.");
    }
    appointmentTable[ID_] = apt_;
    log[ID_] += " Added on: " + Utils::getDateTime();
}

void AppointmentManager::editAppointment(int ID_, const Appointment &newAppointment) {
    if (appointmentTable.find(ID_) == appointmentTable.end()) {
        throw std::invalid_argument("Editing failed. Appointment ID " + std::to_string(newAppointment.getID()) + " not found.");
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

void AppointmentManager::loadFromFile(const std::string& path) {
    // clean data before loading
    appointmentTable.clear();
    log.clear();
    IDHandler<Appointment>::resetIDTable(); 

    // check active path, propriate data
    nlohmann::json jArr = Utils::readJsonFromFile(path);
    if (jArr.empty() || !jArr.is_array()) {
        qDebug() << "[INFO] Appointment file is empty or invalid format";
        return;
    }

    // start reading and load to memory
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
    
    // Set current ID > maxID
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
        throw; // rethrow for caller (UI layer) show message
    }
}
