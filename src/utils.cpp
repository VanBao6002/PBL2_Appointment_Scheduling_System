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
    ss << "                 PRESCRIPTION                    \n";
    ss << "=============================================\n\n";

    ss << "Prescription ID: " << prescriptionID << "\n";
    ss << "Prescription date: " << prescriptionDate.toString() << "\n";
    ss << "Patient ID: " << patientID << "\n";
    ss << "Doctor ID: " << doctorID << "\n\n";

    ss << "Diagnosis: " << diagnosis << "\n\n";

    ss << "Medication list:\n";
    ss << "---------------------------------------------\n";
    for (const auto& med : medicines) {
        ss << "- " << med.first << "\n";  // Tên thuốc
        ss << "  Dosage frequency: " << med.second.first << " times per day\n";  // Tần suất
        ss << "  Duration of treatment: " << med.second.second << " days\n\n";  // Thời gian
    }

    if (!additionalNotes.empty()) {
        ss << "Note: " << additionalNotes << "\n";
    }

    ss << "=============================================\n";
    ss << "Printed on: " << getDateTime() << "\n"; // Ngày in đơn
    ss << "Doctor's signature:\n\n"; // Chữ ký bác sĩ

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

void Utils::validSpecialization(const std::string &specialization_){
    static std::unordered_set<std::string> specializationTable = []{
        std::unordered_set<std::string> set;
        std::ifstream file(Config::SPECIALIZATION_PATH);
        std::string line;
        while (std::getline(file, line)) set.insert(line);
        return set;
    }();
    if (specializationTable.find(specialization_) == specializationTable.end()){
        throw std::invalid_argument("Specialization: " + specialization_ + " is not valid");
    }
}

void Utils::validBloodType(const std::string &bloodType_){
    static std::unordered_set<std::string> bloodTypeTable = []{
        std::unordered_set<std::string> set;
        std::ifstream file(Config::BLOOD_TYPE_PATH);
        std::string line;
        while (std::getline(file, line)) set.insert(line);
        return set;
    }();
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
    static std::unordered_set<std::string> roomTable = []{
        std::unordered_set<std::string> set;
        std::ifstream file(Config::ROOM_PATH);
        std::string line;
        while (std::getline(file, line)) set.insert(line);
        return set;
    }();
    if (roomTable.find(room_) == roomTable.end()){
        throw std::invalid_argument("Room: " + room_ + " is not valid");
    }
}

void Utils::validPrescription(const Prescription &prescription_) {
    if (prescription_.getPrescriptionID() <= 0) {
        throw std::invalid_argument("Invalid prescription ID");
    }
    validID(prescription_.getPatientID());
    validID(prescription_.getDoctorID());

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

void Utils::validMedicalRecord(const MedicalRecord &medicalRecord_) {
    // Kiểm tra ID
    if (medicalRecord_.getRecordID() <= 0) {
        throw std::invalid_argument("Invalid medical record ID");
    }
    validID(medicalRecord_.getPatientID());
    validID(medicalRecord_.getDoctorID());

    // Kiểm tra ngày tạo và cập nhật
    validDate(medicalRecord_.getCreationDate());
    validDate(medicalRecord_.getLastUpdated());

    // Kiểm tra thời gian hợp lệ (ngày cập nhật phải sau ngày tạo)
    if (medicalRecord_.getLastUpdated() < medicalRecord_.getCreationDate()) {
        throw std::invalid_argument("Last updated date cannot be before creation date");
    }

    // Kiểm tra thông tin y tế cơ bản
    if (medicalRecord_.getDiagnosis().empty()) {
        throw std::invalid_argument("Diagnosis cannot be empty");
    }

    if (medicalRecord_.getSymptoms().empty()) {
        throw std::invalid_argument("Symptoms cannot be empty");
    }

    // Kiểm tra các chỉ số sinh tồn
    // Huyết áp (format: "120/80")
    std::string bp = medicalRecord_.getBloodPressure();
    if (!bp.empty()) {  // Nếu có đo huyết áp
        size_t pos = bp.find('/');
        if (pos == std::string::npos) {
            throw std::invalid_argument("Invalid blood pressure format (should be systolic/diastolic)");
        }
        int systolic = std::stoi(bp.substr(0, pos));
        int diastolic = std::stoi(bp.substr(pos + 1));
        
        if (systolic < 70 || systolic > 250 || diastolic < 40 || diastolic > 150) {
            throw std::invalid_argument("Blood pressure values out of reasonable range");
        }
    }

    // Nhịp tim (bình thường: 60-100 nhịp/phút)
    int heartRate = medicalRecord_.getHeartRate();
    if (heartRate != 0 && (heartRate < 40 || heartRate > 200)) {
        throw std::invalid_argument("Heart rate out of reasonable range (40-200 bpm)");
    }

    // Nhiệt độ cơ thể (bình thường: 36.5-37.5°C)
    float temp = medicalRecord_.getBodyTemperature();
    if (temp != 0 && (temp < 35.0 || temp > 42.0)) {
        throw std::invalid_argument("Body temperature out of reasonable range (35-42°C)");
    }

    // Kiểm tra thông tin điều trị
    if (!medicalRecord_.getTreatment().empty()) {
        // Nếu có treatment thì phải có ghi chú của bác sĩ
        if (medicalRecord_.getDoctorNotes().empty()) {
            throw std::invalid_argument("Doctor notes required when treatment is specified");
        }
    }

    // Kiểm tra đơn thuốc (nếu có)
    const auto& prescriptions = medicalRecord_.getPrescriptions();
    for (const auto& prescription : prescriptions) {
        validPrescription(prescription);
    }

    // Kiểm tra lịch tái khám (nếu có)
    const auto& followUpDates = medicalRecord_.getFollowUpDates();
    for (const auto& followUpDate : followUpDates) {
        validDate(followUpDate);
        // Ngày tái khám phải sau ngày tạo hồ sơ
        if (followUpDate <= medicalRecord_.getCreationDate()) {
            throw std::invalid_argument("Follow-up date must be after record creation date");
        }
    }

    // Kiểm tra lịch sử thay đổi
    const auto& history = medicalRecord_.getChangeHistory();
    if (history.empty()) {
        throw std::invalid_argument("Change history cannot be empty - at least creation record needed");
    }
}

void Utils::writeJsonToFile(const std::string& filePath, const nlohmann::json& j){
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file \"" << filePath << "\" for writing.\n";
        return;
    }
    outFile << j.dump(4);
}

nlohmann::json Utils::readJsonFromFile(const std::string& filePath){
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file \"" << filePath << "\" for reading.\n";
        return nlohmann::json();
    }
    std::stringstream ss;
    ss << inFile.rdbuf();
    return nlohmann::json::parse(ss.str());
}

void Utils::writeTextToFile(const std::string& filePath, const std::string& text){
    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open file \"" << filePath << "\" for writing.\n";
        return;
    }
    for (const auto& line : text) {
        outFile << line << '\n';
    }
}

std::string Utils::readTextFromFile(const std::string& filePath){
    std::ifstream inFile(filePath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file \"" << filePath << "\" for reading.\n";
        return "";
    }
    std::ostringstream ss;
    ss << inFile.rdbuf();
    return ss.str();
}

