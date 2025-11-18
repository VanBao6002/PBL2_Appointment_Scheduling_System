#include "appointment.h"

Appointment::Appointment(): doctorID(0), patientID(0), date(), time("00::00::00"), room("00A"), status(Status::Scheduled) {
    int ID = static_cast<int>(IDHandler<Appointment>::generateID());
    setID(ID);
}

Appointment::Appointment(int doctorID, int patientID, const std::string& date_, const std::string& time_, const std::string& room_, const std::string& status_) {
    
    setDoctor(doctorID);
    setPatient(patientID);
    setDate(date_);
    setTime(time_);
    setRoom(room_);
    setStatus(status_);

    int ID = static_cast<int>(IDHandler<Appointment>::generateID());
    setID(ID);
}

void Appointment::setID(int ID_) {
    ID = ID_;
}

void Appointment::setDate(const std::string& date_){
    Utils::validDate(Date::fromString(Utils::trimmed(date_)));
    date = Date::fromString(Utils::trimmed(date_));
}

void Appointment::setTime(const std::string &time_){
    Utils::validTime(Utils::trimmed(time_));
    time = Utils::trimmed(time_);
}

void Appointment::setStatus(const std::string& status_){
    status = statusFromString(Utils::trimmed(status_));
}

void Appointment::setDoctor(int doctorID_){
    if (!IDHandler<Doctor>::checkDuplicate(doctorID_)) {
        throw std::invalid_argument("Doctor ID is not found.");
    }
    doctorID = doctorID_;
}

void Appointment::setPatient(int patientID_){
    if (!IDHandler<Patient>::checkDuplicate(patientID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    patientID = patientID_;
}

void Appointment::setRoom(const std::string room_){
    Utils::validRoom(Utils::trimmed(room_));
    room = Utils::trimmed(room_);
}

int Appointment::getID() const {
    return ID;
}

int Appointment::getDoctorID() const {
    return doctorID;
}

int Appointment::getPatientID() const {
    return patientID;
}

std::string Appointment::getDateTime() const{
    return date.toString() + " " + time;
}

Appointment::Status Appointment::getStatus() const{
    return status;
}

const Doctor& Appointment::getDoctor(const DoctorManager& mgr) const{
    return mgr.getDoctorByID(doctorID);
}

const Patient& Appointment::getPatient(const PatientManager& mgr) const{
    return mgr.getPatientByID(patientID);
}

Appointment::Status Appointment::statusFromString(const std::string& str){
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "occupied") return Appointment::Status::Occupied;
    if (lowerStr == "scheduled") return Appointment::Status::Scheduled;
    if (lowerStr == "canceled") return Appointment::Status::Canceled;
    throw std::invalid_argument("Unknown status: " + str);
}

nlohmann::json Appointment::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["doctorID"] = doctorID;
    j["patientID"] = patientID;
    j["date"] = {
        {"day", date.getDay()},
        {"month", date.getMonth()},
        {"year", date.getYear()}
    };
    j["time"] = time;
    j["room"] = room;
    j["status"] = status;
    return j;
}

void Appointment::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("doctorID")) doctorID = j.at("doctorID").get<int>();
    if (j.contains("patientID")) patientID = j.at("patientID").get<int>();
    if (j.contains("date")) {
        auto dt = j.at("date");
        int d = dt.value("day", 1);
        int m = dt.value("month", 1);
        int y = dt.value("year", 2000);
        date = Date(d, m, y);
    }
    if (j.contains("time")) time = j.at("time").get<std::string>();
    if (j.contains("room")) room = j.at("room").get<std::string>();
    if (j.contains("status")) status = statusFromString(j.at("status").get<std::string>());
}