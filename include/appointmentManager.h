#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "appointment.h"

class AppointmentManager{
    private:    
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Appointment> appointmentTable;

    public:
        AppointmentManager() = default;
        ~AppointmentManager() = default;

        void addAppointment(const Appointment  &doc_);
        void editAppointment(int ID_, const Appointment &newAppointment);
        void removeAppointment(int ID_);
        void changeStatus(int ID_, Appointment::Status status_);

        const Appointment& getAppointmentByID(int ID_) const;
        const std::unordered_map<int, Appointment>& getAllAppointments() const; 
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;
};
