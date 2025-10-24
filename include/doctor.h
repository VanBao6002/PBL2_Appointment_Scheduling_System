#pragma once

#include "person.h"
#include <string>
#include <unordered_set>
class Doctor : public Person {
    public:
        enum class Status {Unavailable, Available}; //encapsulate enum inside doctor class

    private:
        std::string specialization; // Chuyển Khoa
        std::unordered_set<int> patientIDs; // ID bệnh nhân được bác sĩ đảm nhận
        Status doctorStatus;
    
    public:
        Doctor();

        Doctor(int ID_, const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, Status doctorStatus_);

        virtual ~Doctor() = default;

        //getters
        std::string getSpecialization() const {return specialization;}
        const std::unordered_set<int>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;

        //setters
        void setSpecialization(const std::string &specialization_);
        void setStatus(Status doctorStatus_);

        void addPatientID(int patientID_);
        void removePatientID(int patientID_);
};        
