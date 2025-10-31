#pragma once

#include <string>
#include <vector>
#include "date.h"

class Prescription {
    private:
        // Thông tin cơ bản của đơn thuốc
        int prescriptionID;
        int patientID;
        int doctorID;
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
        std::string diagnosis;           // Chẩn đoán bệnh
        std::string additionalNotes;     // Ghi chú thêm của bác sĩ
        bool isActive;                   // Đơn thuốc còn hiệu lực hay không

    public:
        Prescription();
        Prescription(int prescriptionID, int patientID, int doctorID);

        int getPrescriptionID() const;
        int getPatientID() const;
        int getDoctorID() const;
        Date getPrescriptionDate() const;
        const std::vector<Medicine> &getMedicines() const;
        std::string getDiagnosis() const;
        std::string getAdditionalNotes() const;
        bool getIsActive() const;

        void setPrescriptionDate(const Date& date);
        void setDiagnosis(const std::string& diagnosis);
        void setAdditionalNotes(const std::string& notes);
        void setIsActive(bool active);

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
};