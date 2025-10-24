#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "user.h"
#include "medicalRecord.h"
#include "prescription.h"
#include <string>
#include <stdexcept>
#include <unordered_set>
#include <vector>

class Utils {
public:
    static bool isLeapYear(int year);
    static bool isExpired();

    static int calculateTotalDays();
    static double calculateTotalCost();
    static std::string getDateTime();
    static std::string hashFunc(const std::string&);
    static std::string generatePrescriptionText();

    static void validName(const std::string &name);
    static void validGender(char gender);
    static void validDate(const Date &date_);
    static void validID(int ID);
    static void validTime(const std::string &time);

    static void validPatientID(const std::unordered_map<int, Patient> &patientIDs, int patientID_);
    static void validDoctorID(const std::unordered_map<int, Doctor> &doctorsList_, int ID_);
    static void validUserID(const std::unordered_map<int, User> &userTable_, int ID_);
    static void validSpecialization(const std::string &specialization_);
    static void validBloodType(const std::string &bloodType_);
    static void validUserName(const std::string &username_);
    static void validPassword(const std::string &password_);

    template<typename T>
    static T loadFromFile(const std::string &filename, std::unordered_map<int, T> &dataMap);

    template<typename U>
    static void saveToFile(const std::string &filename, const std::unordered_map<int, U> &dataMap);
};

