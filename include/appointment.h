#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include <string>

class Appointment{
    public:
        enum class Status{Occupied, Scheduled, Canceled};

    private:    
        int ID;
        int doctorID;
        int patientID;
        Date date;
        std::string time;
        std::string room;
        Status status;

    public: 
        Appointment();

        Appointment(int doctorID_, int patientID_, const Date& date_, const std::string& time_, const std::string& room_, Status status_);

        ~Appointment() =default;

        void setDateTime(Date date_, const std::string &time_);
        void setStatus(Status status_);
        void setDoctor(int doctorID_);
        void setPatient(int patientID_);
        void setRoom(const std::string room_);

        
        int getID() const;
        int getDoctorID() const;
        int getPatientID() const;
        std::string getDateTime() const;
        Status getStatus() const;
        const Doctor& getDoctor(const DoctorManager& mgr) const;
        const Patient& getPatient(const PatientManager& mgr) const;
        
        // convertor
        static Appointment::Status statusFromString(const std::string& str);
        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);
    
    private:
        void setID(int ID_); 
};
