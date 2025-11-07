#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include <string>

class Appointment{
    public:
        enum class Status{DaDat, DaKham, Huy};

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

        ~Appointment();

        void setDateTime(Date date_, const std::string &time_);
        void setStatus(Status status_);
        void setDoctor(int doctorID_);
        void setPatient(int patientID_);
        void setRoom(const std::string room_);

        
        int getID() const;
        std::string getDateTime() const;
        Status getStatus() const;
        const Doctor& getDoctor(const DoctorManager& mgr) const;
        const Patient& getPatient(const PatientManager& mgr) const;
};
