#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "user.h"
#include <string>
#include <stdexcept>
#include <unordered_set>
#include <vector>

class Utils {
public:
    static bool isLeapYear(int year);
    static std::string getDateTime();
    static std::string hashFunc(const std::string&);


    static void validName(const std::string &name);
    static void validGender(char gender);
    static void validDate(const Date &date_);
    static void validID(int ID);
    static void validTime(const std::string &time);


    static void validPatientID(const std::unordered_map<int, Patient> &patientIDs, int patientID_);
    static void validDoctorID(const std::unordered_map<int, Doctor> &doctorsList_, int ID_);
    static void validUserID(const std::unordered_map<int, User> &userTable, int ID_);
    static void validSpecialization(const std::string &specialization_);
    static void validBloodType(const std::string &bloodType_);
    static void validUserName(const std::string &username_);
    static void validPassword(const std::string &password_);
    
};

