#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "date.h"
#include "prescription.h"
#include "json.hpp"
#include "utils.h"
class MedicalRecord{
    private:
        int ID;
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
        Date history; // Lịch sử thay đổi

    public:
        MedicalRecord();

        MedicalRecord(int patientID_, int doctorID_, const std::string& creationDate_, const std::string& lastUpdated_,
                      const std::string& diagnosis_, const std::string& symptoms_, const std::string& testResults_,
                      const std::string& bloodPressure_, int heartRate_, float bodyTemperature_,
                      const std::string& treatment_, const std::string& doctorNotes_,
                      const std::vector<Date>& followUpDates_, const std::vector<Prescription>& prescriptions_,
                      const std::string& changeHistory_);

        ~MedicalRecord();

        
        MedicalRecord(const MedicalRecord& other);
        MedicalRecord& operator=(const MedicalRecord& other);
        MedicalRecord(MedicalRecord&& other) noexcept;
        MedicalRecord& operator=(MedicalRecord&& other) noexcept;

        
        int getID() const;
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
        const std::vector<Date>& getFollowUpDates() const;
        const std::vector<Prescription>& getPrescriptions() const;
        Date getChangeHistory() const;
        
        void setID(int ID_);
        void setPatientID(int patientID_);
        void setDoctorID(int doctorID_);
        void setCreationDate(const std::string& creationDate_);
        void setLastUpdated(const std::string& lastUpdated_);
        void setDiagnosis(const std::string& diagnosis);
        void setSymptoms(const std::string& symptoms);
        void setTestResults(const std::string& results);
        void setBloodPressure(const std::string& bp);
        void setHeartRate(int rate);
        void setBodyTemperature(float temp);
        void setTreatment(const std::string& treatment);
        void setDoctorNotes(const std::string& notes);
        void addFollowUpDate(const std::string& date_);
        void setChangeHistory(const std::string& date_);
        void updatePrescriptionsMedicalRecordID(int newMedicalRecordID);
        
        // Thêm prescription vào hồ sơ
        void addPrescription(const Prescription& prescription);

        // convertor
        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);
};
