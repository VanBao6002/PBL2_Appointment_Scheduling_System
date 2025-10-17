#pragma once

#include <string>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
class Utils {
public:
    static bool isLeapYear(int year);

    static void validName(const std::string &name);
    static void validGender(char gender);
    static void validDate(const Date &date_);
    static void validID(int ID);

    static void checkValidPatientID(const std::unordered_set<int> &patientIDs, int patientID_);
    static void checkValidDoctorID(const std::unordered_map<int, Doctor> &doctorsList_, int ID_);
    static void checkValidSpecialization(const std::string &specialization_);
    static void checkValidBloodType(const std::string &bloodType_);

    static std::string getDateTime();
};

