#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include "appointment.h"
#include "appointmentManager.h"
#include "user.h"
#include "userManager.h"
#include "medicalRecord.h"
#include "prescription.h"
#include <string>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <concepts>
#include <type_traits>
#include <fstream>

class Utils {
public:
    static bool isExpired(const Date& prescriptionDate, int duration);

    static int calculateTotalDays();
    static double calculateTotalCost();
    static std::string getDateTime();
    static std::string hashFunc(const std::string&);
    static std::string generatePrescriptionText(
        int prescriptionID, 
        const Date& prescriptionDate,
        int patientID, 
        int doctorID, 
        const std::string& diagnosis,
        const std::vector<std::pair<std::string, std::pair<int, int>>>& medicines,
        const std::string& additionalNotes = "");

    static void validName(const std::string &name);
    static void validGender(char gender);
    static void validDate(const Date &date_);
    static void validID(int ID);
    static void validTime(const std::string &time);

    static void validPatientID(const std::unordered_map<int, Patient> &patientIDs, int ID_);
    static void validDoctorID(const std::unordered_map<int, Doctor> &doctorsList_, int ID_);
    static void validUserID(const std::unordered_map<int, User> &userTable_, int ID_);
    static void validAppointmentID(const std::unordered_map<int, Appointment> &appointmentTable_, int ID_);
    static void validSpecialization(const std::string &specialization_);
    static void validBloodType(const std::string &bloodType_);
    static void validUserName(const std::string &username_);
    static void validPassword(const std::string &password_);
    static void validRoom(const std::string &room_);
    static void validPrescription(const Prescription &prescription_);
    static void validMedicalRecord(const MedicalRecord &medicalrecord_);
};

