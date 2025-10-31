#pragma once

#include <string>
#include <vector>
#include "date.h"
#include "prescription.h"
class MedicalRecord{
    private:
        int recordID;
        int patientID;
        int doctorID;
        Date creationDate;
        Date lastUpdated;
        
        // Thông tin y tế
        std::string diagnosis;        // Chẩn đoán
        std::string symptoms;         // Triệu chứng
        std::string testResults;      // Kết quả xét nghiệm
        std::string bloodPressure;    // Huyết áp
        int heartRate;               // Nhịp tim
        float bodyTemperature;       // Nhiệt độ cơ thể

        // Thông tin điều trị
        std::string treatment;        // Phương pháp điều trị
        std::string doctorNotes;      // Ghi chú của bác sĩ
        std::vector<Date> followUpDates;  // Lịch tái khám
        std::vector<Prescription> prescriptions; // Danh sách đơn thuốc
        std::vector<std::string> changeHistory; // Lịch sử thay đổi

    public:
        MedicalRecord();
        MedicalRecord(int recordID_, int patientID_, int doctorID_);

        int getRecordID() const;
        int getPatientID() const;
        int getDoctorID() const;
        Date getCreationDate() const;
        Date getLastUpdated() const;
        std::string getDiagnosis() const;
        std::string getSymptoms() const;
        std::string getTestResults() const;
        std::string getBloodPressure() const;
        int getHeartRate() const;
        float getBodyTemperature() const;
        std::string getTreatment() const;
        std::string getDoctorNotes() const;
        std::vector<Date> getFollowUpDates() const;
        const std::vector<Prescription> &getPrescriptions() const;
        const std::vector<std::string> &getChangeHistory() const;

        void setDiagnosis(const std::string& diagnosis);
        void setSymptoms(const std::string& symptoms);
        void setTestResults(const std::string& results);
        void setBloodPressure(const std::string& bp);
        void setHeartRate(int rate);
        void setBodyTemperature(float temp);
        void setTreatment(const std::string& treatment);
        void setDoctorNotes(const std::string& notes);
        void addFollowUpDate(const Date& date);
};