#include "all.h"
#include <iostream>
using namespace std;

int main (){
    nlohmann::json jArr = Utils::readJsonFromFile("data/doctor.json");
    vector<Doctor> doctorList;
    for (const auto& jDoctor : jArr) {
        Doctor doc;
        doc.fromJson(jDoctor);
        doctorList.push_back(doc);
    }
    DoctorManager mgr;
    
    for (const auto& doctor : doctorList){
        mgr.addDoctor(doctor);
    }



    
}