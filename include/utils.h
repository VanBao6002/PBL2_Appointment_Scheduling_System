#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <stdexcept>
#include <unordered_set>
#include "date.h"
#include "doctor.h"
class Utils {
public:
    static void validName(const std::string &name);
    static void validGender(char gender);
    static bool isLeapYear(int year);
    static void validBirthday(const Date &birthday);
    static void validID(int ID);
    static void checkExistPatientID(const std::unordered_set<int> &patientIDs, int patientID_);
    static void checkExistSpecialization(const std::string &specialization_);
    static void checkValidBloodType(const std::string &bloodType_);
};

#endif 