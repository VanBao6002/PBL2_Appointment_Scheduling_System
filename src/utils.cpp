#include "utils.h"
#include "config.h"
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <string>
#include <ctime>
#include <fstream>
#include <sstream>
#include <algorithm>

bool Utils::isExpired(const Date& prescriptionDate, int duration) {
    std::time_t t = std::time(nullptr);
    std::tm* currentTime = std::localtime(&t);
    Date currentDate(currentTime->tm_year + 1900, 
                    currentTime->tm_mon + 1, 
                    currentTime->tm_mday);
    Date expiryDate = prescriptionDate;
    expiryDate.addDays(duration);
    return currentDate > expiryDate;
}

int Utils::calculateTotalDays() {
    std::ifstream file(Config::PRESCRIPTION_PATH);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open prescription file");
    }

    int maxDuration = 0;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string name;
        int frequency, duration;
        if (iss >> name >> frequency >> duration) {
            if (duration > maxDuration) {
                maxDuration = duration;
            }
        }
    }
    file.close();
    return maxDuration;
}

double Utils::calculateTotalCost() {
    std::ifstream priceFile(Config::MEDICINE_PRICE_PATH);
    if (!priceFile.is_open()) {
        throw std::runtime_error("Cannot open medicine price file");
    }

    std::unordered_map<std::string, double> medicinePrice;
    std::string name;
    double price;
    while (priceFile >> name >> price) {
        medicinePrice[name] = price;
    }
    priceFile.close();

    std::ifstream prescFile(Config::PRESCRIPTION_PATH);
    if (!prescFile.is_open()) {
        throw std::runtime_error("Cannot open prescription file");
    }

    double totalCost = 0.0;
    std::string line;
    while (std::getline(prescFile, line)) {
        std::istringstream iss(line);
        std::string medicineName;
        int frequency, duration;
        if (iss >> medicineName >> frequency >> duration) {
            if (medicinePrice.find(medicineName) != medicinePrice.end()) {
                totalCost += medicinePrice[medicineName] * frequency * duration;
            }
        }
    }
    prescFile.close();

    return totalCost;
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

std::string Utils::generatePrescriptionText(int prescriptionID, const Date& prescriptionDate, 
    int patientID, int doctorID, const std::string& diagnosis,
    const std::vector<std::pair<std::string, std::pair<int, int>>>& medicines,
    const std::string& additionalNotes) {
    
    std::stringstream ss;
    
    ss << "=============================================\n";
    ss << "              ĐƠN THUỐC                    \n";
    ss << "=============================================\n\n";

    ss << "Mã đơn thuốc: " << prescriptionID << "\n";
    ss << "Ngày kê đơn: " << prescriptionDate.toString() << "\n";
    ss << "Mã bệnh nhân: " << patientID << "\n";
    ss << "Mã bác sĩ: " << doctorID << "\n\n";

    ss << "Chẩn đoán: " << diagnosis << "\n\n";

    ss << "DANH SÁCH THUỐC:\n";
    ss << "---------------------------------------------\n";
    for (const auto& med : medicines) {
        ss << "- " << med.first << "\n";  // Tên thuốc
        ss << "  Số lần uống: " << med.second.first << " lần/ngày\n";  // Tần suất
        ss << "  Thời gian điều trị: " << med.second.second << " ngày\n\n";  // Thời gian
    }

    if (!additionalNotes.empty()) {
        ss << "Ghi chú: " << additionalNotes << "\n";
    }

    ss << "=============================================\n";
    ss << "Ngày kê đơn: " << getDateTime() << "\n";
    ss << "Chữ ký bác sĩ:\n\n";

    return ss.str();
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

    int maxDay = date.getDaysInMonth(date.month, date.year);

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

void Utils::validPatientID(const std::unordered_map<int, Patient> &patientIDs, int patientID_){
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
    std::ifstream file(Config::SPECIALIZATION_PATH);
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
    std::ifstream file(Config::BLOOD_TYPE_PATH);
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

void Utils::validRoom(const std::string &room_){
    std::unordered_set<std::string> roomTable;
    std::ifstream file(Config::ROOM_PATH);
    if (!file.is_open()){
        throw std::runtime_error("Failed to open room.txt");
    }
    std::string line;
    while (std::getline(file, line)) {
        roomTable.insert(line);
    }
    file.close();

    if (roomTable.find(room_) == roomTable.end()){
        throw std::invalid_argument("Room: " + room_ + " is not valid");
    }
}

void Utils::validPrescription(const Prescription &prescription_) {
    // Kiểm tra ID
    if (prescription_.getPrescriptionID() <= 0) {
        throw std::invalid_argument("Invalid prescription ID");
    }
    validID(prescription_.getPatientID());
    validID(prescription_.getDoctorID());

    // Kiểm tra ngày kê đơn
    validDate(prescription_.getPrescriptionDate());

    if (prescription_.getDiagnosis().empty()) {
        throw std::invalid_argument("Diagnosis cannot be empty");
    }

    const auto& medicines = prescription_.getMedicines();
    if (medicines.empty()) {
        throw std::invalid_argument("Prescription must have at least one medicine");
    }

    for (const auto& medicine : medicines) {
        if (medicine.name.empty()) {
            throw std::invalid_argument("Medicine name cannot be empty");
        }

        if (medicine.dosage.empty()) {
            throw std::invalid_argument("Medicine dosage cannot be empty");
        }

        if (medicine.frequency <= 0 || medicine.frequency > 6) {
            throw std::invalid_argument("Invalid frequency: must be between 1 and 6 times per day");
        }

        if (medicine.duration <= 0 || medicine.duration > 90) {
            throw std::invalid_argument("Invalid duration: must be between 1 and 90 days");
        }

        if (medicine.instruction.empty()) {
            throw std::invalid_argument("Medicine instruction cannot be empty");
        }
    }
}