#include "medicalRecord.h"

MedicalRecord::MedicalRecord() : patientID(0), doctorID(0), creationDate(Date()), lastUpdated(Date()), diagnosis(""), symptoms(""), testResults(""), bloodPressure(""), heartRate(0), bodyTemperature(0.0f), treatment(""), doctorNotes(""), followUpDates(), prescriptions(), history(Date()) {

    int ID = static_cast<int>(IDHandler<MedicalRecord>::generateID());
    setID(ID);
}

MedicalRecord::MedicalRecord(int patientID_, int doctorID_, const std::string& creationDate_, const std::string& lastUpdated_, const std::string& diagnosis_, const std::string& symptoms_, const std::string& testResults_, const std::string& bloodPressure_, int heartRate_, float bodyTemperature_, const std::string& treatment_, const std::string& doctorNotes_, const std::vector<Date>& followUpDates_, const std::vector<Prescription>& prescriptions_, const std::string& changeHistory_) {

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
    followUpDates = followUpDates_;
    prescriptions = prescriptions_;
    setChangeHistory(changeHistory_);

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
const std::vector<Date>& MedicalRecord::getFollowUpDates() const {
    return followUpDates;
}
const std::vector<Prescription>& MedicalRecord::getPrescriptions() const {
    return prescriptions;
}
Date MedicalRecord::getChangeHistory() const {
    return history;
}

void MedicalRecord::setID(int ID_) {
    ID = ID_;
}

void MedicalRecord::setPatientID(int patientID_) {
    if (!IDHandler<Patient>::checkDuplicate(patientID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    patientID = patientID_;
}

void MedicalRecord::setDoctorID(int doctorID_) {
    if (!IDHandler<Doctor>::checkDuplicate(doctorID_)) {
        throw std::invalid_argument("Doctor ID is not found.");
    }
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
    if (rate < 30 || rate > 220) throw std::out_of_range("Heart rate out of realistic range.");
    heartRate = rate;
}
void MedicalRecord::setBodyTemperature(float temp) {
    if (temp < 30.0f || temp > 45.0f) throw std::out_of_range("Body temperature out of realistic range.");
    bodyTemperature = temp;
}
void MedicalRecord::addFollowUpDate(const std::string& date_) {
    Utils::validDate(Date::fromString(Utils::trimmed(date_)));
    followUpDates.push_back(Date::fromString(Utils::trimmed(date_))); 
}

void MedicalRecord::setChangeHistory(const std::string& date_) {
    Utils::validDate(Date::fromString(Utils::trimmed(date_)));
    history = Date::fromString(Utils::trimmed(date_));
}

nlohmann::json MedicalRecord::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["patientID"] = patientID;
    j["doctorID"] = doctorID;
    j["creationDate"] = creationDate.toString();
    j["lastUpdated"] = lastUpdated.toString();
    j["diagnosis"] = diagnosis;
    j["symptoms"] = symptoms;
    j["testResults"] = testResults;
    j["bloodPressure"] = bloodPressure;
    j["heartRate"] = heartRate;
    j["bodyTemperature"] = bodyTemperature;
    j["treatment"] = treatment;
    j["doctorNotes"] = doctorNotes;
    j["followUpDates"] = Utils::datesToJson(followUpDates);
    j["prescriptions"] = Utils::prescriptionsToJson(prescriptions);
    j["changeHistory"] = Utils::datesToJson(changeHistory);
    return j;
}

void MedicalRecord::fromJson(const nlohmann::json &j) {
    setID(j.at("ID").get<int>());
    setPatientID(j.at("patientID").get<int>());
    setDoctorID(j.at("doctorID").get<int>());
    setCreationDate(j.at("creationDate").get<std::string>());
    setLastUpdated(j.at("lastUpdated").get<std::string>());
    setDiagnosis(j.at("diagnosis").get<std::string>());
    setSymptoms(j.at("symptoms").get<std::string>());
    setTestResults(j.at("testResults").get<std::string>());
    setBloodPressure(j.at("bloodPressure").get<std::string>());
    setHeartRate(j.at("heartRate").get<int>());
    setBodyTemperature(j.at("bodyTemperature").get<float>());
    setTreatment(j.at("treatment").get<std::string>());
    setDoctorNotes(j.at("doctorNotes").get<std::string>());
    followUpDates = Utils::jsonToDates(j.at("followUpDates"));
    prescriptions = Utils::jsonToPrescriptions(j.at("prescriptions"));
    changeHistory = Utils::jsonToDates(j.at("changeHistory"));
}

