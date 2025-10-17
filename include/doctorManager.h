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
        void addDoctor(int ID_, const Doctor  &doc_);
        void editDoctor(int ID_, const Doctor &newDoctor);
        void removeDoctor(int ID_);

        // getters
        Doctor getDoctorByID(int ID_) const;
        std::vector<Doctor> findDoctorsByName(const std::string& name) const;
        const std::unordered_map<int, Doctor>& getAllDoctors() const; // return list of all doctors
        const std::unordered_set<int>& getPatientsByDoctorID(int ID_) const; // return list of patients that being cared by that doctor
};
