#pragma once

#include "person.h"
#include "patient.h"
#include "patientManager.h"
#include "doctor.h"
#include "doctorManager.h"
#include "appointment.h"
#include "appointmentManager.h"
#include "user.h"
#include "userManager.h"
#include "config.h"
#include "json.hpp"
#include "utils.h"

class Core {
    PatientManager patientMgr;
    DoctorManager doctorMgr;
    AppointmentManager appointmentMgr;
    UserManager userMgr;
    
public:
    void loadAll();
    void saveAll();
};