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
        
        std::string diagnosis;       
        std::string symptoms;         
        std::string testResults;      
        std::string bloodPressure;    
        int heartRate;           
        float bodyTemperature;      

        std::string treatment;   
        std::string doctorNotes; 
        std::vector<Date> followUpDates; 
        std::vector<Prescription> prescriptions; 
        Date history; 

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
        
        void addPrescription(const Prescription& prescription);

        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);
};
