#include "core.h"

void Core::loadAll(){
    userMgr.loadFromFile(Config::USER_PATH);
    doctorMgr.loadFromFile(Config::DOCTOR_PATH);
    patientMgr.loadFromFile(Config::PATIENT_PATH);
    appointmentMgr.loadFromFile(Config::APPOINTMENT_PATH);
}

void Core::saveAll(){
    doctorMgr.saveToFile(Config::DOCTOR_PATH);
    patientMgr.saveToFile(Config::PATIENT_PATH);
    appointmentMgr.saveToFile(Config::APPOINTMENT_PATH);
    userMgr.saveToFile(Config::USER_PATH);
}
