#include "prescription.h"
#include "utils.h"
#include <iostream>

Prescription::Prescription():
    prescriptionID(0), patientID(0), doctorID(0), prescriptionDate(Date()), 
    diagnosis(""), additionalNotes(""), isActive(true) {}

Prescription::Prescription(int prescriptionID_, int patientID_, int doctorID_): 
    prescriptionID(prescriptionID_), patientID(patientID_), doctorID(doctorID_), 
    prescriptionDate(Date()), diagnosis(""), additionalNotes(""), isActive(true) {}

// Getters
