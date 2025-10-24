#include "appointment.h"
#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include "utils.h"
#include <string>


void Appointment::setID(int ID_){
    Utils::validID(ID_);
    ID = ID_;
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

void Appointment::setDoctor(const DoctorManager& mgr, int doctorID_){
    Utils::validDoctorID(mgr.getAllDoctors(), doctorID_);
    doctorID = doctorID_;
}

void Appointment::setPatient(const PatientManager& mgr, int patientID_){
    Utils::validPatientID(mgr.getAllPatientsTable(), patientID_);
    patientID = patientID_;
}

void Appointment::setRoom(const std::string room_){
    Utils::validRoom(room_);
    room = room_;
}

//getters
int Appointment::getID() const{
    return ID;
}

std::string Appointment::getDateTime() const{
    return date.toString() + " " + time;
}

Appointment::Status Appointment::getStatus() const{
    return status;
}

Doctor Appointment::getDoctor(const DoctorManager& mgr) const{
    return mgr.getDoctorByID(doctorID);
}

Patient Appointment::getPatient(const PatientManager& mgr) const{
    return mgr.getPatientByID(patientID);
}