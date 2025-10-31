#include "medicalRecord.h"
#include <iostream>

MedicalRecord::MedicalRecord(): 
    recordID(0), patientID(0), doctorID(0), creationDate(Date()), lastUpdated(Date()),
    diagnosis(""), symptoms(""), testResults(""), bloodPressure(""), heartRate(0),
    bodyTemperature(0.0f), treatment(""), doctorNotes(""), followUpDates() {}
MedicalRecord::MedicalRecord(int recordID_, int patientID_, int doctorID_): 
    recordID(recordID_), patientID(patientID_), doctorID(doctorID_), creationDate(Date()), 
    lastUpdated(Date()), diagnosis(""), symptoms(""), testResults(""), bloodPressure(""),
    heartRate(0), bodyTemperature(0.0f), treatment(""), doctorNotes(""), followUpDates() {}

int MedicalRecord::getRecordID() const {
    return recordID;
}
int MedicalRecord::getPatientID() const {
    return patientID;
}
int MedicalRecord::getDoctorID() const {
    return doctorID;
}
Date MedicalRecord::getCreationDate() const {
    return creationDate;
}
Date MedicalRecord::getLastUpdated() const {
    return lastUpdated;
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
std::vector<Date> MedicalRecord::getFollowUpDates() const {
    return followUpDates;
}
const std::vector<Prescription> &MedicalRecord::getPrescriptions() const {
    return prescriptions;
}
const std::vector<std::string> &MedicalRecord::getChangeHistory() const {
    return changeHistory;
}

void MedicalRecord::setDiagnosis(const std::string& diagnosis_) {
    diagnosis = diagnosis_;
}
void MedicalRecord::setSymptoms(const std::string& symptoms_) {
    symptoms = symptoms_;
}
void MedicalRecord::setTestResults(const std::string& results) {
    testResults = results;
}
void MedicalRecord::setBloodPressure(const std::string& bp) {
    bloodPressure = bp;
}
void MedicalRecord::setHeartRate(int rate) {
    heartRate = rate;
}
void MedicalRecord::setBodyTemperature(float temp) {
    bodyTemperature = temp;
}
void MedicalRecord::setTreatment(const std::string& treatment_) {
    treatment = treatment_;
}
void MedicalRecord::setDoctorNotes(const std::string& notes) {
    doctorNotes = notes;
}
void MedicalRecord::addFollowUpDate(const Date& date) {
    followUpDates.push_back(date);
}