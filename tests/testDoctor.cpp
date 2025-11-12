#include "all.h"

int main (){
    // Minimal Doctor test
    // If Doctor::Status is an enum, use:
    Doctor d1("Dr. Alice", 'F', Date::fromString("2006-02-14"), "Cardiology", Doctor::Status::Available);
    assert(d1.getName() == "Dr. Alice");
    d1.setSpecialization("Neurologist");
    assert(d1.getSpecialization() == "Neurologist");
    d1.setStatus(Doctor::Status::Unavailable);
    assert(d1.getStatus() == Doctor::Status::Unavailable);
    DoctorManager mgr;
    mgr.addDoctor(d1);
    Doctor d2;
    mgr.editDoctor(100, d2);
    mgr.findDoctorsByName("Dr.Alice");
}