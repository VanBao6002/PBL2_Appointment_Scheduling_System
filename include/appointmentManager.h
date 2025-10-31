#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "appointment.h"

class AppointmentManager{
    private:    
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Appointment> AppointmentTable;

    public:
        AppointmentManager() = default;
        ~AppointmentManager() = default;

        void addAppointment(int ID_, const Appointment  &doc_);
        void editAppointment(int ID_, const Appointment &newAppointment);
        void removeAppointment(int ID_);
        void changeStatus(int ID_, Appointment::Status status_);

        const Appointment& getAppointmentByID(int ID_) const;
        std::vector<Appointment> findAppointmentsByName(const std::string& name) const;
        const std::unordered_map<int, Appointment>& getAllAppointments() const; 
        const std::unordered_set<int>& getPatientsByAppointmentID(int ID_) const; 
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

        void loadFromFile(const std::string& filename);
        void saveToFile(const std::string& filename) const;
};
