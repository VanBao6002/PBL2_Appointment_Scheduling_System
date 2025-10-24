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

std::string Utils::getDateTime() {
    char buffer[17]; // "YYYY-MM-DD HH:MM" + null terminator
    std::time_t t = std::time(nullptr);
    std::tm* currentDateTime = std::localtime(&t);
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", currentDateTime);
    return std::string(buffer);
}

std::string Utils::hashFunc(const std::string &password_){
    std::string result;
    for (auto c : password_){
        result += std::to_string(c % password_.size());
    } 
    return result;
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

void Utils::validDate(const Date &date) {
    std::time_t t = std::time(nullptr);
    std::tm* currentTime = std::localtime(&t);
    int currentYear = currentTime->tm_year + 1900;
    int lowerYearLimit = currentYear - 200;

    if (date.month < 1 || date.month > 12){
        throw std::invalid_argument("Invalid month");
    }

    int maxDay = 0;
    switch(date.month){
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: 
            maxDay = 31; break;
        case 4: case 6: case 9: case 11: 
            maxDay = 30; break;
        case 2:
            if (isLeapYear(date.year)) {
                maxDay = 29; 
            } else {
                maxDay = 28; 
            }
            break;
        default:
            throw std::invalid_argument("Invalid month");
    }

    if (date.day < 1 || date.day > maxDay){
        throw std::invalid_argument("Invalid day");
    }

    if (date.year < lowerYearLimit || date.year > currentYear){
        throw std::invalid_argument("Invalid year, out of range");
    }
}

void Utils::validID(int ID) {
    if (ID < 0){
        throw std::invalid_argument("Invalid ID, ID must be non-negative");
    }
}

void Utils::validTime(const std::string &time){
    if (time.size() != 5 || time[2] != ':') {
        throw std::invalid_argument("Time format must be HH:MM");
    }
    int hour, minute;
    try {
        hour = std::stoi(time.substr(0, 2));
        minute = std::stoi(time.substr(3, 2));
    } catch (...) {
        throw std::invalid_argument("Time contains invalid numbers");
    }
    if (hour < 0 || hour > 23) {
        throw std::invalid_argument("Hour must be between 00 and 23");
    }
    if (minute < 0 || minute > 59) {
        throw std::invalid_argument("Minute must be between 00 and 59");
    }
}

void Utils::validPatientID(const std::unordered_set<int> &patientIDs, int patientID_){
    if (patientIDs.find(patientID_) == patientIDs.end()){
        throw std::invalid_argument("patient 's ID: " + std::to_string(patientID_) +  " is not found");
    }
}

void Utils::validDoctorID(const std::unordered_map<int, Doctor> &doctorTable, int ID_){
    if (doctorTable.find(ID_) == doctorTable.end()) {
        throw std::invalid_argument("Doctor ID not found.");
    }
}

void Utils::validUserID(const std::unordered_map<int, User> &userTable, int ID_){
    if (userTable.find(ID_) == userTable.end()) {
        throw std::invalid_argument("User ID not found.");
    }
}

void Utils::validSpecialization(const std::string &specialization_){
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
        throw std::invalid_argument("Specialization: " + specialization_ + " is not valid");
    }
}

void Utils::validBloodType(const std::string &bloodType_){
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
        throw std::invalid_argument("BloodType: " + bloodType_ + " is not valid");
    }
}

void Utils::validUserName(const std::string &username_){
    if (username_.size() < 4 || username_.size() > 20){
        throw std::invalid_argument("Username: " + username_ + " is not valid");
    }
}

void Utils::validPassword(const std::string &password_){
    if (password_.size() < 8 || password_.size() > 64){
        throw std::invalid_argument("Password: " + password_ + " is not valid");
    }
}