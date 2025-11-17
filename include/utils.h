#pragma once

#include <string>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <concepts>
#include <type_traits>
#include <fstream>

#include "medicalRecord.h"
#include "prescription.h"

#include "date.h"
#include "config.h"
#include "json.hpp"


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
    static std::string trimmed(const std::string &s);


    //validicator
    static void validName(const std::string &name);
    static void validGender(char gender);
    static void validDate(const Date &date);
    static void validID(int ID);
    static void validTime(const std::string &time);
    static void validSpecialization(const std::string &specialization_);
    static void validBloodType(const std::string &bloodType_);
    static void validUserName(const std::string &username_);
    static void validPassword(const std::string &password_);
    static void validRoom(const std::string &room_);
    static void validPrescription(const Prescription &prescription_);
    static void validMedicalRecord(const MedicalRecord &medicalrecord_);
    static void validPhoneNumber(const std::string &phoneNumber_);

    //convertor
    static void writeJsonToFile(const std::string& filePath, const nlohmann::json& j);
    static nlohmann::json readJsonFromFile(const std::string& filePath);

    static void writeTextToFile(const std::string& filePath, const std::string& text);
    static std::string readTextFromFile(const std::string& filePath);
};

