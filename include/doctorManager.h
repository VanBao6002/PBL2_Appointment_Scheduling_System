#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include "doctor.h"

class DoctorManager{
    private:    
        std::vector<int> doctorsList;
    public:
        DoctorManager() = default;
        ~DoctorManager() = default;

        // modifiers
        void addDoctor(int ID_);
        void editDoctor(int ID_);
        void removeDoctor(int ID_);

        // getters
        int getDoctorByID(int ID_) const;
        std::vector<int> getAllDoctors() const; // return list of all doctors
        std::unordered_set<int> getPatientsByDoctorID(int ID_) const; // return list of patients that being cared by that doctor
};
