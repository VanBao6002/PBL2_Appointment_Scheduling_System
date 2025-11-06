#include "appointmentManager.h"
#include "utils.h"
#include <algorithm>
#include <stdexcept>

void AppointmentManager::addAppointment(int ID_, const Appointment &apt_) {
    if (AppointmentTable.find(ID_) != AppointmentTable.end()) {
        throw std::invalid_argument("Appointment ID already exists.");
    }
    AppointmentTable[ID_] = apt_;
    log[ID_] += " Added on " + Utils::getDateTime();
}

void AppointmentManager::editAppointment(int ID_, const Appointment &newAppointment) {
    if (AppointmentTable.find(ID_) == AppointmentTable.end()) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    AppointmentTable[ID_] = newAppointment;
    log[ID_] += " Edited on " + Utils::getDateTime();
}

void AppointmentManager::removeAppointment(int ID_) {
    if (AppointmentTable.find(ID_) == AppointmentTable.end()) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    AppointmentTable.erase(ID_);
    log.erase(ID_);
}

void AppointmentManager::changeStatus(int ID_, Appointment::Status status_) {
    if (AppointmentTable.find(ID_) == AppointmentTable.end()) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    AppointmentTable[ID_].setStatus(status_);
    log[ID_] += " Status changed on " + Utils::getDateTime();
}

const Appointment& AppointmentManager::getAppointmentByID(int ID_) const {
    if (AppointmentTable.find(ID_) == AppointmentTable.end()) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    return AppointmentTable.at(ID_);
}

std::vector<Appointment> AppointmentManager::findAppointmentsByName(const std::string& name) const {
    std::vector<Appointment> result;
    for (const auto& [id, apt] : AppointmentTable) {
        // giả sử tìm theo tên bệnh nhân, cần PatientManager để lấy tên
        result.push_back(apt);
    }
    return result;
}

const std::unordered_map<int, Appointment>& AppointmentManager::getAllAppointments() const {
    return AppointmentTable;
}

const std::unordered_map<int, std::string>& AppointmentManager::getAllLog() const {
    return log;
}

const std::string& AppointmentManager::getIDLog(int ID_) const {
    if (AppointmentTable.find(ID_) == AppointmentTable.end()) {
        throw std::invalid_argument("Appointment ID not found.");
    }
    return log.at(ID_);
}
