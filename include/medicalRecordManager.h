#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>

#include "medicalRecord.h"


class MedicalRecordManager {
    private:    
        std::unordered_map<int, std::string> log;
        std::unordered_map<int, MedicalRecord> medicalRecordTable;

        MedicalRecordManager() = default;
        MedicalRecordManager(const MedicalRecordManager&) = delete;
        MedicalRecordManager& operator=(const MedicalRecordManager&) = delete;
    public:
        static MedicalRecordManager& getInstance() {
                    static MedicalRecordManager instance;
                    return instance;
        }
        ~MedicalRecordManager() = default;

        void addMedicalRecord(const MedicalRecord  &record_);
        void editMedicalRecord(int ID_, const MedicalRecord &newMedicalRecord);
        void removeMedicalRecord(int ID_);

        const MedicalRecord& getMedicalRecordByID(int ID_) const;
        const std::unordered_map<int, MedicalRecord>& getAllMedicalRecords() const; 

        const std::unordered_map<int, std::string>& getAllLog() const;
        const std::string& getIDLog(int ID_) const;

        void loadFromFile(const std::string& path);
        void saveToFile(const std::string& path);
};
