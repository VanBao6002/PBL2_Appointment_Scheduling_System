#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <algorithm>
#include <vector>

#include "person.h"
#include "patient.h"

class Doctor : public Person{
    public:
        enum class Status {Unavailable, Available};

    private:
        std::string specialization; // Chuyên Khoa
        std::vector<int> patientIDs; // ID bệnh nhân được bác sĩ đảm nhận
        Status doctorStatus;
        std::string phoneNumber;
        std::string email;
    
    public:
        Doctor();

        Doctor(const std::string& name_, char gender_, const std::string& birthday_, const std::string &phoneNumber_, const std::string& specialization_, const std::string& doctorStatus_, const std::string& email_);

        ~Doctor() = default;

        void setSpecialization(const std::string &specialization_);
        void setStatus(const std::string& doctorStatus_);
        void setEmail(const std::string &email_);


        void addPatientID(int ID_);
        void removePatientID(int ID_);

        std::string getSpecialization() const {return specialization;}
        Status getStatus() const {return doctorStatus;}
        const std::vector<int>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;
        std::string getEmail() const {return email;}

        // convertor
        static Doctor::Status statusFromString(const std::string& str);
        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &j) override;
};        
