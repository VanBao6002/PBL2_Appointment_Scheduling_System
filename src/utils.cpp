#include "utils.h"
#include "doctor.h"
#include "patient.h"
#include "prescription.h"
#include <regex>

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
        throw std::runtime_error("Cannot open prescription file.");
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
        throw std::runtime_error("Cannot open medicine price file.");
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
        throw std::runtime_error("Cannot open prescription file.");
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
    std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %M:%H", currentDateTime);
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

std::string Utils::trimmed (const std::string &str) {
    std::string trimmed = str;
    trimmed.erase(trimmed.begin(), std::find_if(trimmed.begin(), trimmed.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    trimmed.erase(std::find_if(trimmed.rbegin(), trimmed.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), trimmed.end());
    return trimmed;
}

std::string Utils::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c){ return std::tolower(c); });
    return result;
}

void Utils::validName(const std::string &name_) {
    std::string trimmed = Utils::trimmed(name_);

    if (trimmed.empty()) {
        throw std::invalid_argument("Name must not be empty or only whitespace.");
    }

    for (char c : trimmed) {
        if (std::isdigit(static_cast<unsigned char>(c))) {
            throw std::invalid_argument("Name cannot contain numbers.");
        }
    }
}

void Utils::validGender(char gender) {
    if (std::tolower(gender) != 'm' && std::tolower(gender) != 'f'){
        throw std::invalid_argument("Invalid gender, must be 'M' or 'F'.");
    }
}

void Utils::validDate(const Date &date) {
    std::time_t t = std::time(nullptr);
    std::tm* currentTime = std::localtime(&t);
    int currentYear = currentTime->tm_year + 1900;
    int lowerYearLimit = currentYear - 200;

    if (date.month < 1 || date.month > 12){
        throw std::invalid_argument("Invalid month.");
    }

    int maxDay = date.getDaysInMonth(date.month, date.year);

    if (date.day < 1 || date.day > maxDay){
        throw std::invalid_argument("Invalid day.");
    }

    if (date.year < lowerYearLimit || date.year > currentYear){
        throw std::invalid_argument("Invalid year, out of range.");
    }
}

void Utils::validPhoneNumber(const std::string &phoneNumber) {
    if (phoneNumber.size() != 11) throw std::invalid_argument("Invalid phonenumber.");
}

void Utils::validID(int ID) {
    if (ID < 0){
        throw std::invalid_argument("Invalid ID, ID must be non-negative.");
    }
}

void Utils::validTime(const std::string &time){
    if (time.size() != 5 || time[2] != ':') {
        throw std::invalid_argument("Time format must be HH:MM.");
    }
    int hour, minute;
    try {
        hour = std::stoi(time.substr(0, 2));
        minute = std::stoi(time.substr(3, 2));
    } catch (...) {
        throw std::invalid_argument("Time contains invalid numbers.");
    }
    if (hour < 0 || hour > 23) {
        throw std::invalid_argument("Hour must be between 00 and 23.");
    }
    if (minute < 0 || minute > 59) {
        throw std::invalid_argument("Minute must be between 00 and 59.");
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
        throw std::invalid_argument("Specialization: " + specialization_ + " is not valid.");
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
        throw std::invalid_argument("BloodType: " + bloodType_ + " is not valid.");
    }
}

void Utils::validUserName(const std::string &username_){
    Utils::validName(username_);
    if (username_.size() < 4 || username_.size() > 20){
        throw std::invalid_argument("Username: " + username_ + " is not valid.");
    }
}

void Utils::validPassword(const std::string &password_){
    if (password_.size() < 8 || password_.size() > 64){
        throw std::invalid_argument("Password: " + password_ + " is not valid.");
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
        throw std::invalid_argument("Room: " + room_ + " is not valid.");
    }
}

void Utils::validPrescription(const Prescription &prescription_) {
    validID(prescription_.getID());
    validID(prescription_.getMedicalRecordID());

    validDate(prescription_.getDate());

    const auto& medicines = prescription_.getMedicines();
    if (medicines.empty()) {
        throw std::invalid_argument("Prescription must have at least one medicine.");
    }

    for (const auto& medicine : medicines) {
        if (medicine.name.empty()) {
            throw std::invalid_argument("Medicine name cannot be empty.");
        }

        if (medicine.dosage.empty()) {
            throw std::invalid_argument("Medicine dosage cannot be empty.");
        }

        if (medicine.frequency <= 0 || medicine.frequency > 6) {
            throw std::invalid_argument("Invalid frequency: must be between 1 and 6 times per day.");
        }

        if (medicine.duration <= 0 || medicine.duration > 90) {
            throw std::invalid_argument("Invalid duration: must be between 1 and 90 days.");
        }

        if (medicine.instruction.empty()) {
            throw std::invalid_argument("Medicine instruction cannot be empty.");
        }
    }
}

void Utils::validEmail(const std::string& email) {
    // Trim khoảng trắng 2 đầu
    std::string e = Utils::trimmed(email);
    const std::regex pattern(
        R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"
    );
    if (!std::regex_match(e, pattern)) {
        throw std::invalid_argument("Invalid email format: " + email);
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



