#include "doctor.h"
#include "utils.h"
#include "doctorManager.h"
#include <algorithm>
#include <unordered_set>

void DoctorManager::addDoctor(int ID_, Doctor doc_) {
    if (Utils::checkValidDoctorID(doctorTable, ID_)) {
        doctorTable.insert({ID_, doc_});
        log.insert({ID_, Utils::getDate()});
        return;
    }
    throw std::invalid_argument("Failed adding doctor.");
}

void DoctorManager::editDoctor(int ID_){
    if (Utils::checkValidDoctorID(doctorTable, ID_)) {
        
        return;
    }
    throw std::invalid_argument("Failed adding doctor.");
}
                                                                                                                                               
void DoctorManager::removeDoctor(int ID_){

}


int DoctorManager::getDoctorByID(int ID_) const{

}

std::vector<int> DoctorManager::getAllDoctors() const{

}

std::unordered_set<int> DoctorManager::getPatientsByDoctorID(int ID_) const{
    
}
