#include "appointment.h"
#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include "utils.h"
#include "IDHandler.h"
#include <string>
#include <limits>

Appointment::Appointment(): ID(static_cast<int>(IDHandler<Appointment>::generateID())), doctorID(0), patientID(0), date(), time(""), room(""), status(Status::DaDat) {
    IDHandler<Appointment>::registerID(ID);
}

Appointment::Appointment(int doctorID_, int patientID_, const Date& date_, const std::string& time_, const std::string& room_, Status status_ = Status::DaDat): ID(static_cast<int>(IDHandler<Appointment>::generateID())), doctorID(doctorID_), patientID(patientID_), date(date_), time(time_), room(room_), status(status_) {
    IDHandler<Appointment>::registerID(ID);
}

Appointment::~Appointment() {
    IDHandler<Appointment>::unregisterID(ID);
}

void Appointment::setDateTime(Date date_, const std::string &time_){
    Utils::validDate(date_);
    Utils::validTime(time_);
    date = date_;
    time = time_;
}

void Appointment::setStatus(Appointment::Status status_){
    status = status_;
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
    Utils::validRoom(room_);
    room = room_;
}

int Appointment::getID() const{
    return ID;
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