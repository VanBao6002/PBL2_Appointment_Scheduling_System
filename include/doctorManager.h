#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "doctor.h"

class DoctorManager{
    private:    
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Doctor> doctorTable;

    public:
        DoctorManager() = default;
        ~DoctorManager() = default;

        // modifiers
        void addDoctor(int ID_, Doctor doc_);
        void editDoctor(int ID_);
        void removeDoctor(int ID_);

        // getters
        int getDoctorByID(int ID_) const;
        std::vector<int> getAllDoctors() const; // return list of all doctors
        std::unordered_set<int> getPatientsByDoctorID(int ID_) const; // return list of patients that being cared by that doctor
};
