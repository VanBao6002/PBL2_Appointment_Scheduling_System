#pragma once

#include <iostream>
#include <vector>
#include "patient.h"

class PatientManager{
    private:
        std::vector<int> patientsList;
    public:
        PatientManager() = default;
        ~PatientManager() = default;

        // modifiers
        void addPatient(int ID_);
        void editPatient(int ID_);
        void removePatient(int ID_);

        // getters
        int getPatientByID(int ID_) const;
        std::vector<int> getAllPatients() const; // return list of all patients
};