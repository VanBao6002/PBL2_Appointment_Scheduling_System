#ifndef DOCTOR_H
#define DOCTOR_H

#include "person.h"
#include <string>
#include <unordered_set>
class Doctor : public Person {
    public:
        enum status { Unavailable, Available, OnLeave }; //encapsulate enum inside doctor class

    private:
        std::string specialization; // chuyen khoa
        std::unordered_set<int> patientIDs; // ID benh nhan duoc bac si dam nhan
        status doctorStatus;
    
    public:
        Doctor();

        Doctor(const std::string &name_, char gender_, const Date &birthday_, int ID_, const std::string specialization_, status doctorStatus_);

        ~Doctor() = default;

        void displayInfo() const override;
        //getters
        std::string getSpecialization() const {return specialization;}
        const std::unordered_set<int>& getPatientIDs() const {return patientIDs;}

        //setters
        void setSpecialization(const std::string &specialization_);
        void setStatus(status doctorStatus_);

        void addPatientID(int patientID_);
        void removePatientID(int patientID_);
};        
#endif