#include "medicalRecord.h"
#include <iostream>

MedicalRecord::MedicalRecord()
    : patientID(0), doctorID(0), creationDate(), lastUpdated(),
      diagnosis(""), symptoms(""), testResults(""), bloodPressure(""),
      heartRate(0), bodyTemperature(0.0f), treatment(""), doctorNotes(""),
      followUpDates(), prescriptions(), changeHistory() 
{
    int ID = static_cast<int>(IDHandler<MedicalRecord>::generateID());
    setID(ID);
}

MedicalRecord::MedicalRecord(int patientID_, int doctorID_, const std::string& creationDate_, const std::string& lastUpdated_,
    const std::string& diagnosis_, const std::string& symptoms_, const std::string& testResults_,
    const std::string& bloodPressure_, int heartRate_, float bodyTemperature_,
    const std::string& treatment_, const std::string& doctorNotes_,
    const std::vector<Date>& followUpDates_, const std::vector<Prescription>& prescriptions_,
    const std::vector<std::string>& changeHistory_) {

    setPatientID(patientID_);
    setDoctorID(doctorID_);
    setCreationDate(creationDate_);
    setLastUpdated(lastUpdated_);
    setDiagnosis(diagnosis_);
    setSymptoms(symptoms_);
    setTestResults(testResults_);
    setBloodPressure(bloodPressure_);
    setHeartRate(heartRate_);
    setBodyTemperature(bodyTemperature_);
    setTreatment(treatment_);
    setDoctorNotes(doctorNotes_);

    int ID = static_cast<int>(IDHandler<MedicalRecord>::generateID());
    setID(ID);
}


int MedicalRecord::getID() const {
    return ID;
}
int MedicalRecord::getPatientID() const {
    return patientID;
}
int MedicalRecord::getDoctorID() const {
    return doctorID;
}
std::string MedicalRecord::getDiagnosis() const {
    return diagnosis;
}
std::string MedicalRecord::getSymptoms() const {
    return symptoms;
}
std::string MedicalRecord::getTestResults() const {
    return testResults;
}
std::string MedicalRecord::getBloodPressure() const {
    return bloodPressure;
}
int MedicalRecord::getHeartRate() const {
    return heartRate;
}
float MedicalRecord::getBodyTemperature() const {
    return bodyTemperature;
}
std::string MedicalRecord::getTreatment() const {
    return treatment;
}
std::string MedicalRecord::getDoctorNotes() const {
    return doctorNotes;
}
std::vector<std::string> MedicalRecord::getFollowUpDates() const {
    return followUpDates;
}
const std::vector<Prescription> &MedicalRecord::getPrescriptions() const {
    return prescriptions;
}
const std::vector<std::string> &MedicalRecord::getChangeHistory() const {
    return changeHistory;
}

void MedicalRecord::setID(int ID_) {
    ID = ID_;
}

void MedicalRecord::setPatientID(int patientID_) {
    patientID = patientID_;
}

void MedicalRecord::setDoctorID(int doctorID_) {
    doctorID = doctorID_;
}

void MedicalRecord::setCreationDate(const std::string& creationDate_) {
    Utils::validDate(Date::fromString(Utils::trimmed(creationDate_)));
    creationDate = Date::fromString(Utils::trimmed(creationDate_));
}

void MedicalRecord::setLastUpdated(const std::string& lastUpdated_) {
    Utils::validDate(Date::fromString(Utils::trimmed(lastUpdated_)));
    lastUpdated = Date::fromString(Utils::trimmed(lastUpdated_));
}

void MedicalRecord::setDiagnosis(const std::string& diagnosis_) {
    diagnosis = Utils::trimmed(diagnosis_);
}
void MedicalRecord::setSymptoms(const std::string& symptoms_) {
    symptoms = Utils::trimmed(symptoms_);
}
void MedicalRecord::setTestResults(const std::string& results) {
    testResults = Utils::trimmed(results);
}
void MedicalRecord::setBloodPressure(const std::string& bp) {
    bloodPressure = Utils::trimmed(bp);
}
void MedicalRecord::setTreatment(const std::string& treatment_) {
    treatment = Utils::trimmed(treatment_);
}
void MedicalRecord::setDoctorNotes(const std::string& notes) {
    doctorNotes = Utils::trimmed(notes);
}
void MedicalRecord::setHeartRate(int rate) {
    heartRate = rate; // Consider adding validation if needed
}
void MedicalRecord::setBodyTemperature(float temp) {
    bodyTemperature = temp; // Consider adding validation if needed
}
void MedicalRecord::addFollowUpDate(const std::string& date) {
    std::string trimmedDate = Utils::trimmed(date);
    // Optionally validate date format here
    // followUpDates.push_back(Date::fromString(trimmedDate));
}

nlohmann::json MedicalRecord::toJson() const {
    
}

void MedicalRecord::fromJson(const nlohmann::json &j) {
  
}

