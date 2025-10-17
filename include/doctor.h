#pragma once

#include "person.h"
#include <string>
#include <unordered_set>
class Doctor : public Person {
    public:
        enum class Status {Unavailable, Available}; //encapsulate enum inside doctor class

    private:
        std::string specialization; // chuyen khoa
        std::unordered_set<int> patientIDs; // ID benh nhan duoc bac si dam nhan
        Status doctorStatus;
    
    public:
        Doctor();

        Doctor(const std::string& name_, char gender_, const Date& birthday_, int ID_, const std::string& specialization_, Status doctorStatus_);

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
