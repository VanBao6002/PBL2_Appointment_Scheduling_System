#include "prescription.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iostream>

Prescription::Prescription():
    prescriptionID(0), patientID(0), doctorID(0), prescriptionDate(Date()), 
    diagnosis(""), additionalNotes(""), isActive(true) {}

Prescription::Prescription(int prescriptionID_, int patientID_, int doctorID_): 
    prescriptionID(prescriptionID_), patientID(patientID_), doctorID(doctorID_), 
    prescriptionDate(Date()), diagnosis(""), additionalNotes(""), isActive(true) {}

int Prescription::getPrescriptionID() const {
    return prescriptionID;
}
int Prescription::getPatientID() const {
    return patientID;
}
int Prescription::getDoctorID() const {
    return doctorID;
}
Date Prescription::getPrescriptionDate() const {
    return prescriptionDate;
}
const std::vector<Prescription::Medicine> &Prescription::getMedicines() const {
    return medicines;
}
std::string Prescription::getDiagnosis() const {
    return diagnosis;
}
std::string Prescription::getAdditionalNotes() const {
    return additionalNotes;
}
bool Prescription::getIsActive() const {
    return isActive;
}

void Prescription::setPrescriptionDate(const Date& date) {
    prescriptionDate = date;
}
void Prescription::setDiagnosis(const std::string& diagnosis_) {
    diagnosis = diagnosis_;
}
void Prescription::setAdditionalNotes(const std::string& notes) {
    additionalNotes = notes;
}
void Prescription::setIsActive(bool active) {
    isActive = active;
}

// Quản lý thuốc trong đơn
void Prescription::addMedicine(const std::string& name, 
                             const std::string& dosage,
                             int frequency,
                             int duration,
                             const std::string& instruction) {
    Medicine med = {name, dosage, frequency, duration, instruction};
    for (const auto& m : medicines) { // tránh trùng tên thuốc trong cùng đơn
        if (m.name == name) return; // hoặc update
    }
    medicines.emplace_back(Medicine{name, dosage, frequency, duration, instruction});
}
void Prescription::removeMedicine(const std::string& name) {
    medicines.erase(std::remove_if(medicines.begin(), medicines.end(),
                   [&name](const Medicine& med) { return med.name == name; }),
                   medicines.end());
}
void Prescription::updateMedicineDosage(const std::string& name, const std::string& newDosage) {
    auto it = std::find_if(medicines.begin(), medicines.end(),
                        [&name](const Medicine& med){ return med.name == name; });
    if (it != medicines.end()) {
        it->dosage = newDosage; // thay đổi trường tương ứng
    } else {
        throw std::runtime_error("Medicine not found");
    }
}
void Prescription::updateMedicineFrequency(const std::string& name, int newFrequency) {
    auto it = std::find_if(medicines.begin(), medicines.end(),
                        [&name](const Medicine& med){ return med.name == name; });
    if (it != medicines.end()) {
        it->frequency = newFrequency; // thay đổi trường tương ứng
    } else {
        throw std::runtime_error("Medicine not found");
    }
}
void Prescription::updateMedicineDuration(const std::string& name, int newDuration) {
    auto it = std::find_if(medicines.begin(), medicines.end(),
                        [&name](const Medicine& med){ return med.name == name; });
    if (it != medicines.end()) {
        it->duration = newDuration; // thay đổi trường tương ứng
    } else {
        throw std::runtime_error("Medicine not found");
    }
}
void Prescription::updateMedicineInstruction(const std::string& name, const std::string& newInstruction) {
    auto it = std::find_if(medicines.begin(), medicines.end(),
                        [&name](const Medicine& med){ return med.name == name; });
    if (it != medicines.end()) {
        it->instruction = newInstruction; // thay đổi trường tương ứng
    } else {
        throw std::runtime_error("Medicine not found");
    }
}

