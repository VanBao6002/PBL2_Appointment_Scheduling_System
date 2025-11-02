#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "doctor.h"
#include "patient.h"
#include "serializable.h"

class DoctorManager : public ISerializable {
    private:    
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, Doctor> doctorTable;

    public:
        DoctorManager() = default;
        ~DoctorManager() = default;

        void addDoctor(int ID_, const Doctor  &doc_);
        void editDoctor(int ID_, const Doctor &newDoctor);
        void removeDoctor(int ID_);
        void changeStatus(int ID_, Doctor::Status status_);

        const Doctor& getDoctorByID(int ID_) const;
        std::vector<Doctor> findDoctorsByName(const std::string& name) const;
        const std::unordered_map<int, Doctor>& getAllDoctors() const; // return list of all doctors
        const std::unordered_map<int, Patient>& getPatientsByDoctorID(int ID_) const; // return list of patients that being cared by that doctor
        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

        // File operations
        void loadFromFile(const std::string& filename);
        void saveToFile(const std::string& filename) const;

        // Stream operations
        bool loadFromStream(std::istream& is) override;
        void saveToStream(std::ostream& os) const override;
};
