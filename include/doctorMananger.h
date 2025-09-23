#ifndef DOCTORMANAGER_H
#define DOCTORMANAGER_H
#include <string>
#include <vector>
#include <unordered_set>
#include "doctor.h"
class DoctorManager{
    private:    
        std::vector<Doctor> doctorsList;
    public:
        DoctorManager() = default;
        ~DoctorManager() = default;


        // modifiers
        void addDoctor(const Doctor &doctor_);
        bool editDoctor(const std::string &ID_, const Doctor& updatedDoctor);
        bool removeDoctor(const std::string &ID_);

        // getters
        Doctor* getDoctorByID(const std::string &ID_) const;
        std::vector<Doctor> getAllDoctors() const; // return list of all doctors
        std::unordered_set<int> getPatientsByDoctorID(const std::string &ID_) const; // return list of patients that being cared by that doctor
};
#endif