#pragma once

#include <string>
#include <limits>

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include "utils.h"
#include "IDHandler.h"
#include "json.hpp"


class Appointment{
    public:
        enum class Status{Occupied, Scheduled, Canceled};

    private:    
        int ID;
        int doctorID;
        int patientID;
        Date date;
        std::string startTime;
        std::string endTime;
        std::string room;
        Status status;

    public: 
        Appointment();

        Appointment(int doctorID, int patientID, const std::string& date_, const std::string& startTime_, const std::string& endTime_, const std::string& room_, const std::string& status_);

        ~Appointment();

        
        Appointment(const Appointment& other);
        Appointment& operator=(const Appointment& other);
        Appointment(Appointment&& other) noexcept;
        Appointment& operator=(Appointment&& other) noexcept;
        
        void setID(int ID_); 
        void setDate(const std::string& date_);
        void setStartTime(const std::string &startTime_);
        void setEndTime(const std::string &endTime_);
        void setStatus(const std::string& status_);
        void setDoctorID(int doctorID_);
        void setPatientID(int patientID_);
        void setRoom(const std::string room_);

        
        int getID() const;
        int getDoctorID() const;
        int getPatientID() const;
        const Date& getDate() const;
        std::string getRoom() const {return room;}
        std::string getStartTime() const {return startTime;}
        std::string getEndTime() const {return endTime;}
        std::string getDuration() const {
            int startHour = std::stoi(startTime.substr(0, 2));
            int startMin = std::stoi(startTime.substr(3, 2));
            int endHour = std::stoi(endTime.substr(0, 2));
            int endMin = std::stoi(endTime.substr(3, 2));
            int duration = (endHour * 60 + endMin) - (startHour * 60 + startMin);
            return std::to_string(duration);
        }
        Status getStatus() const;
        const Doctor& getDoctor(const DoctorManager& mgr) const;
        const Patient& getPatient(const PatientManager& mgr) const;
        
        // convertor
        static Appointment::Status statusFromString(const std::string& str);
        static std::string statusToString(Status status);
        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);
};
