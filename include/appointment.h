#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include "serializable.h"
#include <string>

class Appointment : public ISerializable {
    public:
        enum class Status{DaDat, DaKham, Huy};

    private:    
        int ID; // Mã lịch hẹn (ID, duy nhất)
        int doctorID; // Bác sĩ (ID bác sĩ, tên bác sĩ, chuyên khoa)
        int patientID; // Bệnh nhân (ID bệnh nhân, tên bệnh nhân)
        Date date; // Ngày tháng năm của lịch hẹn
        std::string time; // Giờ của lịch hẹn
        std::string room; // Phòng khám (số phòng hoặc khoa)
        Status status; // Trạng thái lịch hẹn (đã đặt, đã khám, hủy)

    public: 
        Appointment() = default;
        ~Appointment() = default;

        void setID(int ID_);
        void setDateTime(Date date_, const std::string &time_);
        void setStatus(Status status_);
        void setDoctor(const DoctorManager& mgr, int doctorID_);
        void setPatient(const PatientManager& mgr, int patientID_);
        void setRoom(const std::string room_);

        
        int getID() const;
        std::string getDateTime() const;
        Status getStatus() const;
        const Doctor& getDoctor(const DoctorManager& mgr) const;
        const Patient& getPatient(const PatientManager& mgr) const;

        void serialize(std::ostream& os) const override;
        void deserialize(std::istream& is) override;

};
