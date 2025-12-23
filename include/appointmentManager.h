#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>


#include "appointment.h"
#include "doctorManager.h"
#include "patientManager.h"
#include "userManager.h"
#include "utils.h" 
#include "IDHandler.h" 
#include "json.hpp" 

class AppointmentManager{
private:
    std::unordered_map<int, std::string> log;
    std::unordered_map<int, Appointment> appointmentTable;


    AppointmentManager() = default;
    AppointmentManager(const AppointmentManager&) = delete;
    AppointmentManager& operator=(const AppointmentManager&) = delete;

public:
    static AppointmentManager& getInstance() {
        static AppointmentManager instance;
        return instance;
    }

    ~AppointmentManager() = default;

    void addAppointment(const Appointment &doc_);
    void editAppointment(int ID_, const Appointment &newAppointment);
    void removeAppointment(int ID_);
    const std::vector<std::pair<std::string, std::string>>& getBookedSlotsForDoctorDate(int selectedDoctorID, const std::string& selectedDate);
    const Appointment& getAppointmentByID(int ID_) const;
    std::vector<Appointment> getAllAppointmentsAsVector() const; 
    const std::unordered_map<int, Appointment>& getAllAppointmentsMap() const;
    const std::unordered_map<int, std::string>& getAllLog() const;
    const std::string& getIDLog(int ID_) const;

    bool checkDuplicateAppointment(const Appointment &apt_);
    void removeDuplicates();
    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path);
};