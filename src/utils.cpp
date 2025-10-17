#include "utils.h"
#include "config.h"
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <algorithm>

bool Utils::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

void Utils::validName(const std::string &name_) {
    if (name_.empty()) {
        throw std::invalid_argument("Name cannot be empty");
    }
}

void Utils::validGender(char gender) {
    if (std::tolower(gender) != 'm' && std::tolower(gender) != 'f'){
        throw std::invalid_argument("Invalid gender, must be 'M' or 'F'");
    }
}

void Utils::validDate(const Date &birthday) {
    std::time_t t = std::time(nullptr);
    std::tm* currentTime = std::localtime(&t);
    int currentYear = currentTime->tm_year + 1900;
    int lowerYearLimit = currentYear - 200;

    if (birthday.month < 1 || birthday.month > 12){
        throw std::invalid_argument("Invalid month");
    }

    int maxDay = 0;
    switch(birthday.month){
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: 
            maxDay = 31; break;
        case 4: case 6: case 9: case 11: 
            maxDay = 30; break;
        case 2:
            if (isLeapYear(birthday.year)) {
                maxDay = 29; 
            } else {
                maxDay = 28; 
            }
            break;
        default:
            throw std::invalid_argument("Invalid month");
    }

    if (birthday.day < 1 || birthday.day > maxDay){
        throw std::invalid_argument("Invalid day");
    }

    if (birthday.year < lowerYearLimit || birthday.year > currentYear){
        throw std::invalid_argument("Invalid year, out of range");
    }
}

void Utils::validID(int ID) {
    if (ID < 0){
        throw std::invalid_argument("Invalid ID, ID must be non-negative");
    }
}

bool Utils::checkValidPatientID(const std::unordered_set<int> &patientIDs, int patientID_){
    if (patientIDs.find(patientID_) == patientIDs.end()){
        throw std::invalid_argument("patient 's ID: " + std::to_string(patientID_) +  " is not found in doctor's list");
        return true;
    }
    return false;
}

bool Utils::checkValidDoctorID(const std::unordered_map<int, Doctor> &doctorTable, int ID_){
    auto it = doctorTable.find(ID_);
    if (it == doctorTable.end()) {
        throw std::invalid_argument("Doctor ID not found.");
        return true;
    }
    return false;
}

bool Utils::checkValidSpecialization(const std::string &specialization_){
    std::unordered_set<std::string> specializationTable;
    std::ifstream file(Config::SPECIALIZATION_FILE);
    if (!file.is_open()){
        throw std::runtime_error("Failed to open doctorSpecializations.txt");
    }
    std::string line;
    while (std::getline(file, line)) {
        specializationTable.insert(line);
    }
    file.close();

    if (specializationTable.find(specialization_) == specializationTable.end()){
        throw std::invalid_argument("specializatio: " + specialization_ + " is not found");
        return true;
    }
    return false;
}

bool Utils::checkValidBloodType(const std::string &bloodType_){
    std::unordered_set<std::string> bloodTypeTable;
    std::ifstream file(Config::BLOOD_TYPE_FILE);
    if (!file.is_open()){
        throw std::runtime_error("Failed to open bloodType.txt");
    }
    std::string line;
    while (std::getline(file, line)) {
        bloodTypeTable.insert(line);
    }
    file.close();

    if (bloodTypeTable.find(bloodType_) == bloodTypeTable.end()){
        throw std::invalid_argument("specializatio: " + bloodType_ + " is not found");
        return true;
    }

    return false;
}

std::string Utils::getDate(){
    char buffer[11];

    std::time_t t = std::time(nullptr);
    std::tm* currentTime = std::localtime(&t);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", currentTime);
    return std::string(buffer);
}