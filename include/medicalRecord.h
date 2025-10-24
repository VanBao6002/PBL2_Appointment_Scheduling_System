#pragma once

#include <string>
#include <vector>
#include "date.h"
class MedicalRecord{
    private:
        // Thông tin cơ bản
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

    public:
        MedicalRecord();
        MedicalRecord(int recordID_, int patientID_, int doctorID_);

        // Getters
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

        // Setters
        void setDiagnosis(const std::string& diagnosis);
        void setSymptoms(const std::string& symptoms);
        void setTestResults(const std::string& results);
        void setBloodPressure(const std::string& bp);
        void setHeartRate(int rate);
        void setBodyTemperature(float temp);
        void setTreatment(const std::string& treatment);
        void setDoctorNotes(const std::string& notes);
        void addFollowUpDate(const Date& date);

        // Các hàm tiện ích
        void updateLastModified(); // Cập nhật lần cuối sửa đổi
        bool isValidRecord() const;

        // Các thao tác với file
        bool saveToFile() const;
        static MedicalRecord loadFromFile(int recordID);
};