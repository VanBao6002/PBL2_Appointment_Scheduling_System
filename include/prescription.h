#pragma once

#include <string>
#include <vector>
#include "date.h"
#include "json.hpp"
class Prescription{
    public:
        enum class Status {Active, Inactive};

    private:
        // Thông tin cơ bản của đơn thuốc
        int ID;
        int medicalRecordID;
        Date prescriptionDate;    // Ngày kê đơn
        
        // Thông tin về thuốc
        struct Medicine {
            std::string name;        // Tên thuốc
            std::string dosage;      // Liều lượng (vd: "500mg")
            int frequency;           // Số lần uống mỗi ngày
            int duration;            // Số ngày uống
            std::string instruction; // Hướng dẫn sử dụng (vd: "Uống sau ăn")
        };
        std::vector<Medicine> medicines;  // Danh sách các thuốc trong đơn        
        std::string additionalNotes;     // Ghi chú thêm của bác sĩ
        Status prescriptionStatus;       // Đơn thuốc còn hiệu lực hay không

    public:
        Prescription();
        Prescription(int medicalRecordID_, const std::string& prescriptionDate_, const std::string& additionalNotes_, const std::string& prescriptionStatus_);
        ~Prescription() = default;
        
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

        // Quản lý thuốc trong đơn
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
        
        // convertor
        static Prescription::Status statusFromString(const std::string& str);
        static std::string statusToString(Status status);
        nlohmann::json toJson() const;
        void fromJson(const nlohmann::json &j);
        
    private:
        void setID(int ID);
};