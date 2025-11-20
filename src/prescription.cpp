#include "prescription.h"
#include "IDHandler.h"
#include "utils.h"
#include <algorithm>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iostream>

Prescription::Prescription(): medicalRecordID(0), prescriptionDate(Date()), medicines(), additionalNotes(""), prescriptionStatus(Status::Active) {
    ID = static_cast<int>(IDHandler<Prescription>::generateID());
    setID(ID);
}

Prescription::Prescription(int medicalRecordID_, const std::string& prescriptionDate_, const std::string& additionalNotes_, const std::string& prescriptionStatus_) {

    setMedicalRecordID(medicalRecordID_);
    setDate(prescriptionDate_);
    setAdditionalNotes(additionalNotes_);
    setStatus(prescriptionStatus_);

    ID = static_cast<int>(IDHandler<Prescription>::generateID());
    setID(ID);
}

int Prescription::getID() const {
    return ID;
}
int Prescription::getMedicalRecordID () const {
    return medicalRecordID;
}
Date Prescription::getPrescriptionDate() const {
    return prescriptionDate;
}
const std::vector<Prescription::Medicine> &Prescription::getMedicines() const {
    return medicines;
}
std::string Prescription::getAdditionalNotes() const {
    return additionalNotes;
}
Prescription::Status Prescription::getPrescriptionStatus() const {
    return prescriptionStatus;
}

void Prescription::setID(int ID_){
    ID = ID_;
}
void Prescription::setMedicalRecordID(int medicalRecordID_) {
    if (IDHandler<MedicalRecord>::checkDuplicate(medicalRecordID_)) {
        throw std::invalid_argument("MedicalRecord ID already existed.");
    }
    medicalRecordID = medicalRecordID_;
}
void Prescription::setDate(const std::string& date_) {
    Utils::validDate(Date::fromString(Utils::trimmed(date_)));
    date = Date::fromString(Utils::trimmed(date_));
}

void Prescription::setAdditionalNotes(const std::string& notes_) {
    Utils::validName(Utils::trimmed(notes_));
    additionalNotes = Utils::trimmed(notes_)
}

void Prescription::setStatus(const std::string& status_){
    status = statusFromString(Utils::trimmed(status_));
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

Prescription::Status Prescription::statusFromString(const std::string& str){
    if (Utils::toLower(str) == "active") return Prescription::Status::Active;
    if (Utils::toLower(str) == "inactive") return Prescription::Status::Inactive;
    throw std::invalid_argument("Unknown status: " + str);
}

std::string Prescription::statusToString(Status status) {
    switch (status) {
        case Status::Active: return "Active";
        case Status::Inactive: return "Inactive";
    }
    return "Unknown";
}

nlohmann::json Prescription::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["medicalRecordID"] = medicalRecordID;
    j["prescriptionDate"] = prescriptionDate.toString();
    j["medicines"] = nlohmann::json::array();
    for (const auto& med : medicines) {
        j["medicines"].push_back({
            {"name", med.name},
            {"dosage", med.dosage},
            {"frequency", med.frequency},
            {"duration", med.duration},
            {"instruction", med.instruction}
        });
    }
    j["additionalNotes"] = additionalNotes;
    j["prescriptionStatus"] = statusToString(prescriptionStatus);
    
    return j;
}
// Deserialize Prescription from JSON
void Prescription::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("medicalRecordID")) medicalRecordID = j.at("medicalRecordID").get<int>();
    if (j.contains("prescriptionDate")) 
        prescriptionDate = Date::fromString(j.at("prescriptionDate").get<std::string>());
    if (j.contains("medicines")) {
        medicines.clear();
        for (const auto& medj : j.at("medicines")) {
            medicines.push_back(Medicine{
                medj.value("name", ""),
                medj.value("dosage", ""),
                medj.value("frequency", 0),
                medj.value("duration", 0),
                medj.value("instruction", "")
            });
        }
    }
    if (j.contains("additionalNotes")) 
        additionalNotes = j.at("additionalNotes").get<std::string>();
    if (j.contains("prescriptionStatus")) 
        prescriptionStatus = statusFromString(j.at("prescriptionStatus").get<std::string>());
    
}
