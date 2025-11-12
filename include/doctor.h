#pragma once

#include "person.h"
#include "patient.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

class Doctor : public Person{
    public:
        enum class Status {Unavailable, Available};

    private:
        std::string specialization; // Chuyên Khoa
        std::unordered_map<int, Patient> patientIDs; // ID bệnh nhân được bác sĩ đảm nhận
        Status doctorStatus;
    
    public:
        Doctor();

        Doctor(const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, Status doctorStatus_);

        ~Doctor();

        void setSpecialization(const std::string &specialization_);
        void setStatus(Status doctorStatus_);

        void addPatientID(const Patient &patientID_);
        void removePatientID(int patientID_);

        std::string getSpecialization() const {return specialization;}
        Status getStatus() const {return doctorStatus;}
        const std::unordered_map<int, Patient>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;

        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &j) override;
};        
