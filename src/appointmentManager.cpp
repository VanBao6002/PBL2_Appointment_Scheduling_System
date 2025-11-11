#include "appointmentManager.h"
#include "utils.h"
#include "IDHandler.h"
#include <algorithm>
#include <stdexcept>

void AppointmentManager::addAppointment(const Appointment &apt_) {
    int ID_ = apt_.getID();
    if (IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Appointment ID already existed.");
    }
    appointmentTable[ID_] = apt_;
    log[ID_] += " Added on: " + Utils::getDateTime();
}

void AppointmentManager::editAppointment(int ID_, const Appointment &newAppointment) {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    appointmentTable[ID_] = newAppointment;
    log[ID_] += " Edited on: " + Utils::getDateTime();
}

void AppointmentManager::removeAppointment(int ID_) {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    appointmentTable.erase(ID_);
    log.erase(ID_);
    IDHandler<Appointment>::unregisterID(ID_);
}

void AppointmentManager::changeStatus(int ID_, Appointment::Status status_) {
    if (appointmentTable.find(ID_) == appointmentTable.end()) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    appointmentTable[ID_].setStatus(status_);
    log[ID_] += " Status changed on: " + Utils::getDateTime();
}

const Appointment& AppointmentManager::getAppointmentByID(int ID_) const {
    if (!IDHandler<Appointment>::checkDuplicate(ID_)) {
        throw std::invalid_argument("Appointment ID not found.");
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
        throw std::invalid_argument("Appointment ID not found.");
    }
    return log.at(ID_);
}
