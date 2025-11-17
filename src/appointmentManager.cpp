#include "appointmentManager.h"

void AppointmentManager::addAppointment(const Appointment &apt_) {
    int ID_ = apt_.getID();
    if (IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Adding failed. Appointment ID " + std::to_string(apt_.getID()) + " already exists.");
    }
    appointmentTable[ID_] = apt_;
    log[ID_] += " Added on: " + Utils::getDateTime();
    IDHandler<Appointment>::registerID(ID_);
}

void AppointmentManager::editAppointment(int ID_, const Appointment &newAppointment) {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Editing failed. Appointment ID " + std::to_string(newAppointment.getID()) + " not found.");
    }
    appointmentTable[ID_] = newAppointment;
    log[ID_] += " Edited on: " + Utils::getDateTime();
}

void AppointmentManager::removeAppointment(int ID_) {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Removing failed. Appointment ID " + std::to_string(ID_) + " not found.");
    }
    appointmentTable.erase(ID_);
    log.erase(ID_);
    IDHandler<Appointment>::unregisterID(ID_);
}

void AppointmentManager::changeStatus(int ID_, Appointment::Status status_) {
    if (appointmentTable.find(ID_) == appointmentTable.end()) {
        throw std::invalid_argument("Cannot change status. Appointment ID " + std::to_string(ID_) + " not found.");
    }
    appointmentTable[ID_].setStatus(status_);
    log[ID_] += " Status changed on: " + Utils::getDateTime();
}

const Appointment& AppointmentManager::getAppointmentByID(int ID_) const {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Failed getting. Appointment ID " + std::to_string(ID_) + " not found.");
    }
    return appointmentTable.at(ID_);
}

const std::unordered_map<int, Appointment>& AppointmentManager::getAllAppointments() const {
    return appointmentTable;
}

const std::unordered_map<int, std::string>& AppointmentManager::getAllLog() const {
    return log;
}

const std::string& AppointmentManager::getIDLog(int ID_) const {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Failed getting log. Appointment ID " +  std::to_string(ID_) + " not found.");
    }
    return log.at(ID_);
}

void AppointmentManager::loadFromFile(const std::string& path) {
    nlohmann::json jArr = Utils::readJsonFromFile(path);

    for (const auto& jAppointment : jArr) {
        Appointment apt;
        apt.fromJson(jAppointment);
        // check relationship between doctor - patient before loading to appointment manager.
        if (!IDHandler<Doctor>::checkDuplicate(apt.getDoctorID())){
            throw std::invalid_argument("Failed loading from: " + path + ". Appointment ID: " + std::to_string(apt.getID()) + ". Doctor ID " + std::to_string(apt.getDoctorID()) + " not found.");
        }

        if (!IDHandler<Patient>::checkDuplicate(apt.getPatientID())){
            throw std::invalid_argument("Failed loading from: " + path + ". Appointment ID: " + std::to_string(apt.getID()) + ". Patient ID " + std::to_string(apt.getPatientID()) + " not found.");
        }
        addAppointment(apt);
    }
}


void AppointmentManager::saveToFile(const std::string& path){
    nlohmann::json jArr;
    for (const auto& pair : appointmentTable) {
        jArr.push_back(pair.second.toJson());   
    }
    Utils::writeJsonToFile(path, jArr);
}