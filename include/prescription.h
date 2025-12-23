#pragma once

#include <string>
#include <vector>
#include "date.h"
#include "json.hpp"
class Prescription{
    public:
        enum class Status {Active, Inactive};
        struct Medicine {
            std::string name;       
            std::string dosage;   
            int frequency;         
            int duration;         
            std::string instruction;
        };
        
    private:
        int ID;
        int medicalRecordID;
        Date prescriptionDate; 
        std::vector<Medicine> medicines;        
        std::string additionalNotes;   
        Status prescriptionStatus;  

    public:
        Prescription();
        Prescription(int medicalRecordID_, const std::string& prescriptionDate_, const std::string& additionalNotes_, const std::string& prescriptionStatus_);
        ~Prescription();
        
        
        Prescription(const Prescription& other);
        Prescription& operator=(const Prescription& other);
        Prescription(Prescription&& other) noexcept;
        Prescription& operator=(Prescription&& other) noexcept;
        

        int getID() const;
        int getMedicalRecordID () const;
        Date getDate() const;
        const std::vector<Medicine> &getMedicines() const;
        std::string getAdditionalNotes() const;
        Prescription::Status getStatus() const;

        void setMedicalRecordID (int medicalRecordID_);
        void setDate(const std::string& date_);
        void setAdditionalNotes(const std::string& notes_);
        void setStatus(const std::string& status_);

        void addMedicine(const std::string& name, 
                        const std::string& dosage,
                        int frequency,
                        int duration,
                        const std::string& instruction);
        void removeMedicine(const std::string& name);
        void updateMedicineDosage(const std::string& name, const std::string& newDosage);
        void updateMedicineFrequency(const std::string& name, int newFrequency);
        void updateMedicineDuration(const std::string& name, int newDuration);
        void updateMedicineInstruction(const std::string& name, const std::string& newInstruction);
        
        static Prescription::Status statusFromString(const std::string& str);
        static std::string statusToString(Status status);
        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);
        
        Medicine* findMed(const std::string& name) {
            for (auto& m : medicines) {
                if (m.name == name) return &m;
            }
            return nullptr;
        }

    private:
        void setID(int ID);
};