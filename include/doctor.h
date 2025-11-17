#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <algorithm>

#include "person.h"
#include "patient.h"

class Doctor : public Person{
    public:
        enum class Status {Unavailable, Available};

    private:
        std::string specialization; // Chuyên Khoa
        std::unordered_map<int, Patient> patientIDs; // ID bệnh nhân được bác sĩ đảm nhận
        Status doctorStatus;
        std::string phoneNumber;
        std::string email;
    
    public:
        Doctor();

        Doctor(const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, const std::string& doctorStatus_, const std::string& phoneNumber_, const std::string& email_);

        ~Doctor() = default;

        void setSpecialization(const std::string &specialization_);
        void setStatus(const std::string& doctorStatus_);
        void setPhoneNumber(const std::string &phonenumber_);
        void setEmail(const std::string &email_);


        void addPatientID(const Patient &patientID_);
        void removePatientID(int patientID_);

        std::string getSpecialization() const {return specialization;}
        Status getStatus() const {return doctorStatus;}
        const std::unordered_map<int, Patient>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;
        std::string getPhoneNumber() const {return phoneNumber;}
        std::string getEmail() const {return email;}

        // convertor
        static Doctor::Status statusFromString(const std::string& str);
        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &j) override;
};        
