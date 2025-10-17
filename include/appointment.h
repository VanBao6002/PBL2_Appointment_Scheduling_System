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
        std::string ID; // Mã lịch hẹn (ID, duy nhất)
        int doctorID; // Bác sĩ (ID bác sĩ, tên bác sĩ, chuyên khoa)
        int patientID; // Bệnh nhân (ID bệnh nhân, tên bệnh nhân)
        Date date; // Ngày tháng năm của lịch hẹn
        std::string time; // Giờ của lịch hẹn
        std::string room; // Phòng khám (số phòng hoặc khoa)
        Status status; // Trạng thái lịch hẹn (đã đặt, đã khám, hủy)

    public: 
        //setters
        void setDateTime(Date date_, const std::string &time_);
        void setStatus(Status status_);
        //getters
        std::string getDateTime();
        Status getStatus();
        Doctor getDoctor(const DoctorManager& mgr) const;
    
};
