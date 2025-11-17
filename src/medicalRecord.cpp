#include "medicalRecord.h"
#include <iostream>

MedicalRecord::MedicalRecord(): ID(0), patientID(0), doctorID(0), creationDate(Date()), lastUpdated(Date()), diagnosis(""), symptoms(""), testResults(""), bloodPressure(""), heartRate(0), bodyTemperature(0.0f), treatment(""), doctorNotes(""), followUpDates() {
    ID = static_cast<int>(IDHandler<MedicalRecord>::generateID());
    setID(ID);
}

MedicalRecord::MedicalRecord(int ID_, int patientID_, int doctorID_): ID(ID_), patientID(patientID_), doctorID(doctorID_), creationDate(Date()), lastUpdated(Date()), diagnosis(""), symptoms(""), testResults(""), bloodPressure(""), heartRate(0), bodyTemperature(0.0f), treatment(""), doctorNotes(""), followUpDates() {
    ID = static_cast<int>(IDHandler<MedicalRecord>::generateID());
    setID(ID);
}


int MedicalRecord::getRecordID() const {
    return ID;
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

void MedicalRecord::setID(int ID_) {
    ID = ID_;
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

nlohmann::json MedicalRecord::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["patientID"] = patientID;
    j["doctorID"] = doctorID;
    j["creationDate"] = {
        {"day", creationDate.getDay()},
        {"month", creationDate.getMonth()},
        {"year", creationDate.getYear()}
    };
    j["lastUpdated"] = {
        {"day", lastUpdated.getDay()},
        {"month", lastUpdated.getMonth()},
        {"year", lastUpdated.getYear()}
    };
    j["diagnosis"] = diagnosis;
    j["symptoms"] = symptoms;
    j["testResults"] = testResults;
    j["bloodPressure"] = bloodPressure;
    j["heartRate"] = heartRate;
    j["bodyTemperature"] = bodyTemperature;
    j["treatment"] = treatment;
    j["doctorNotes"] = doctorNotes;
    j["followUpDates"] = nlohmann::json::array();
    for (const auto& date : followUpDates) {
        j["followUpDates"].push_back({
            {"day", date.getDay()},
            {"month", date.getMonth()},
            {"year", date.getYear()}
        });
    }
    j["prescriptions"] = nlohmann::json::array();
    for (const auto& pres : prescriptions) {
        j["prescriptions"].push_back(pres.toJson());
    }
    j["changeHistory"] = changeHistory;
    return j;
}

void MedicalRecord::fromJson(const nlohmann::json &j) {
    ID = j.value("ID", 0);
    patientID = j.value("patientID", 0);
    doctorID = j.value("doctorID", 0);

    auto cd = j.at("creationDate");
    creationDate = Date(cd.at("day"), cd.at("month"), cd.at("year"));

    auto lu = j.at("lastUpdated");
    lastUpdated = Date(lu.at("day"), lu.at("month"), lu.at("year"));

    diagnosis = j.value("diagnosis", "");
    symptoms = j.value("symptoms", "");
    testResults = j.value("testResults", "");
    bloodPressure = j.value("bloodPressure", "");
    heartRate = j.value("heartRate", 0);
    bodyTemperature = j.value("bodyTemperature", 0.0f);
    treatment = j.value("treatment", "");
    doctorNotes = j.value("doctorNotes", "");

    followUpDates.clear();
    for (const auto& date : j.at("followUpDates")) {
        followUpDates.emplace_back(date.at("day"), date.at("month"), date.at("year"));
    }

    prescriptions.clear();
    for (const auto& pres : j.at("prescriptions")) {
        Prescription p;
        p.fromJson(pres);
        prescriptions.push_back(p);
    }

    changeHistory = j.value("changeHistory", std::vector<std::string>{});
}

