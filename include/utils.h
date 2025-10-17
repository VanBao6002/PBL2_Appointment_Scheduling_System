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
    static void validDate(const Date &birthday);
    static void validID(int ID);

    static bool checkValidPatientID(const std::unordered_set<int> &patientIDs, int patientID_);
    static bool checkValidDoctorID(const std::unordered_map<int, Doctor> &doctorsList_, int ID_);
    static bool checkValidSpecialization(const std::string &specialization_);
    static bool checkValidBloodType(const std::string &bloodType_);

    static std::string getDate();
};

