#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <unordered_set>

#include "patient.h"
#include "userManager.h"
#include "utils.h"
#include "json.hpp"

class PatientManager{
    private:
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Patient> patientTable;

        PatientManager() = default;
        PatientManager(const PatientManager&) = delete;
        PatientManager& operator=(const PatientManager&) = delete;
    public:
        static PatientManager& getInstance() {
                    static PatientManager instance;
                    return instance;
        }
        ~PatientManager() = default;

        void addPatient(const Patient &pat_);
        void editPatient(int ID_, const Patient &newPatient);
        void removePatient(int ID_);
        bool isPatientExist(int patientID) const;
        
        const Patient& getPatientByID(int ID_) const;
        std::vector<Patient> findPatientsByName(const std::string& name) const;
        const std::unordered_map<int, Patient>& getAllPatientsTable() const;
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

        // loader
        void loadFromFile(const std::string& path);
        void saveToFile(const std::string& path);
};