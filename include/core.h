#pragma once

#include "patientManager.h"
#include "doctorManager.h"
#include "appointmentManager.h"
#include "userManager.h"


class Core {
    PatientManager patientMgr;
    DoctorManager doctorMgr;
    AppointmentManager appointmentMgr;
    UserManager userMgr;
    
public:
    void loadAll();
    void saveAll();
};