#include "appointment.h"

Appointment::Appointment(): ID(0), doctorID(0), patientID(0), date(), startTime("00:00"), endTime("01:00") , room("00A"), status(Status::Scheduled) {;
}

Appointment::Appointment(int doctorID, int patientID, const std::string& date_, const std::string& startTime_, const std::string& endTime_, const std::string& room_, const std::string& status_) : ID(0) {
    
    setDoctorID(doctorID);
    setPatientID(patientID);
    setDate(date_);
    setStartTime(startTime_);
    setEndTime(endTime_);
    setRoom(room_);
    setStatus(status_);
}

Appointment::~Appointment(){
}

Appointment::Appointment(const Appointment& other)
    : doctorID(other.doctorID),
      patientID(other.patientID),
      date(other.date),
      startTime(other.startTime),
      endTime(other.endTime),
      room(other.room),
      status(other.status),
      ID(other.ID)
{

}

Appointment& Appointment::operator=(const Appointment& other)
{
    if (this != &other) {
        doctorID = other.doctorID;
        patientID = other.patientID;
        date = other.date;
        startTime = other.startTime;
        endTime = other.endTime;
        room = other.room;
        status = other.status;
        ID = other.ID;
    }
    return *this;
}

Appointment::Appointment(Appointment&& other) noexcept
    : doctorID(other.doctorID),
      patientID(other.patientID),
      date(std::move(other.date)),
      startTime(std::move(other.startTime)),
      endTime(std::move(other.endTime)),
      room(std::move(other.room)),
      status(other.status),
      ID(other.ID)
{
    other.ID = 0;
}

Appointment& Appointment::operator=(Appointment&& other) noexcept
{
    if (this != &other) {
        doctorID = other.doctorID;
        patientID = other.patientID;
        date = std::move(other.date);
        startTime = std::move(other.startTime);
        endTime = std::move(other.endTime);
        room = std::move(other.room);
        status = other.status;
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

void Appointment::setID(int ID_) {
    ID = ID_;
}

void Appointment::setDate(const std::string& date_){
    Utils::validDate(Date::fromString(Utils::trimmed(date_)));
    date = Date::fromString(Utils::trimmed(date_));
}

void Appointment::setStartTime(const std::string &startTime_){
    Utils::validTime(Utils::trimmed(startTime_));
    startTime = Utils::trimmed(startTime_);
}

void Appointment::setEndTime(const std::string &endTime_){
    Utils::validTime(Utils::trimmed(endTime_));
    endTime = Utils::trimmed(endTime_);
}

void Appointment::setStatus(const std::string& status_){
    status = statusFromString(Utils::trimmed(status_));
}

void Appointment::setDoctorID(int doctorID_){
    if (!IDHandler<Doctor>::checkDuplicateID(doctorID_)) {
        throw std::invalid_argument("Doctor ID is not found.");
    }
    doctorID = doctorID_;
}

void Appointment::setPatientID(int patientID_){
    if (!IDHandler<Patient>::checkDuplicateID(patientID_)){
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

const Date& Appointment::getDate() const {
    return date;
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
    if (Utils::toLower(str) == "occupied") return Appointment::Status::Occupied;
    if (Utils::toLower(str) == "scheduled") return Appointment::Status::Scheduled;
    if (Utils::toLower(str) == "canceled") return Appointment::Status::Canceled;
    throw std::invalid_argument("Unknown status: " + str);
}

std::string Appointment::statusToString(Status status) {
    switch (status) {
        case Status::Occupied: return "Occupied";
        case Status::Scheduled: return "Scheduled";
        case Status::Canceled: return "Canceled";
    }
    return "Unknown";
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
    j["startTime"] = startTime;
    j["endTime"] = endTime;
    j["room"] = room;
    j["status"] = statusToString(status);
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
    if (j.contains("startTime")) startTime = j.at("startTime").get<std::string>();
    if (j.contains("endTime")) endTime = j.at("endTime").get<std::string>();
    if (j.contains("room")) room = j.at("room").get<std::string>();
    if (j.contains("status")) status = statusFromString(j.at("status").get<std::string>());
}