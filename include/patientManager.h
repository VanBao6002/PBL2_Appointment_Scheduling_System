#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include "patient.h"

class PatientManager{
    private:
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Patient> patientTable;
    public:
        PatientManager() = default;
        ~PatientManager() = default;

        void addPatient(int ID_, const Patient &pat_);
        void editPatient(int ID_, const Patient &newPatient);
        void removePatient(int ID_);

        const Patient& getPatientByID(int ID_) const;
        std::vector<Patient> findPatientsByName(const std::string& name) const;
        const std::unordered_map<int, Patient>& getAllPatientsTable() const;
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

};