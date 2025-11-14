#include "all.h"
#include <unordered_map>
#include <unordered_set>
#include <iostream>
#include <string>
using namespace std;

void TEST_PRINT_ALL_INFO (DoctorManager& mgr) {
    for (const auto& pair : mgr.getAllDoctors()) {
        cout << pair.second.getInfo() << endl;
    }
}

void TEST_LOAD(DoctorManager& mgr, const string& path){
    mgr.loadFromFile(path);
}

void TEST_ADD(DoctorManager& mgr, Doctor& doc) {
    mgr.addDoctor(doc);
}

void TEST_EDIT(DoctorManager& mgr, Doctor& doc) {
    mgr.editDoctor(doc.getID(), doc);
}

void TEST_REMOVE(DoctorManager& mgr, Doctor& doc) {
    mgr.removeDoctor(doc.getID());
}



int main (){
    DoctorManager mgr;

    Doctor d1;
    d1.setName("Tom");
    TEST_ADD(mgr, d1);
    TEST_PRINT_ALL_INFO(mgr);
    

    d1.setName("Tim");
    TEST_EDIT(mgr, d1);
    TEST_PRINT_ALL_INFO(mgr);


    TEST_REMOVE(mgr, d1);
    TEST_PRINT_ALL_INFO(mgr);


    //TEST_LOAD(mgr, Config::DOCTOR_PATH);

    
}