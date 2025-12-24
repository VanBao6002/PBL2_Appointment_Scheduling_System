#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "doctor.h"
#include "patientManager.h"
#include "userManager.h"
#include "utils.h"
#include "json.hpp"

class DoctorManager{
    private:    
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Doctor> doctorTable;
        std::unordered_map<std::string, int> CCCDToID;

        DoctorManager() = default;
        DoctorManager(const DoctorManager&) = delete;
        DoctorManager& operator=(const DoctorManager&) = delete;

    public:
        static DoctorManager& getInstance() {
            static DoctorManager instance;
            return instance;
        }
        ~DoctorManager() = default;
        void addDoctor(const Doctor  &doc_);
        void editDoctor(int ID_, const Doctor &newDoctor);
        void removeDoctor(int ID_);
        bool isDoctorExist(int doctorID) const;

        const Doctor& getDoctorByID(int ID_) const;
        std::vector<Doctor> findDoctorsByName(const std::string& name) const;
        const std::unordered_map<int, Doctor>& getAllDoctors() const;
        const std::unordered_set<int>& getPatientsByDoctorID(int ID_) const;
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;
        const Doctor& getDoctorByCCCD(const std::string& CCCD) const;
        const std::unordered_map<std::string, int>& getCCCDtoID() const;

        void loadFromFile(const std::string& path);
        void saveToFile(const std::string& path);
};
