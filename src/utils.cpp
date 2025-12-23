#include "utils.h"
#include "doctor.h"
#include "patient.h"
#include "prescription.h"
#include "config.h"
#include <qdebug.h>
#include <qlogging.h>
#include <regex>
#include <ctime>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

bool Utils::isExpired(const Date& prescriptionDate, int duration) {
    std::time_t t = std::time(nullptr);
    std::tm* currentTime = std::localtime(&t);
    Date currentDate(currentTime->tm_mday, 
                    currentTime->tm_mon + 1, 
                    currentTime->tm_year + 1900);
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
    char buffer[20]; 
    std::time_t t = std::time(nullptr);
    std::tm* currentDateTime = std::localtime(&t);
    std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", currentDateTime); 
    return std::string(buffer);
}

std::string Utils::hashFunc(const std::string &password_){
    if (password_.empty()) return "";
    unsigned int hash = 0;
    const int PRIME = 31;

    for (char c : password_) {
        hash = hash * PRIME + static_cast<unsigned char>(c);
    }

    return std::to_string(hash);
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
        ss << "- " << med.first << "\n"; 
        ss << "  Dosage frequency: " << med.second.first << " times per day\n"; 
        ss << "  Duration of treatment: " << med.second.second << " days\n\n"; 
    }

    if (!additionalNotes.empty()) {
        ss << "Note: " << additionalNotes << "\n";
    }

    ss << "=============================================\n";
    ss << "Printed on: " << getDateTime() << "\n"; 
    ss << "Doctor's signature:\n\n"; 

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

std::string Utils::toUpper(const std::string &str) {
    std::string upperStr = str;
    std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), 
        [](unsigned char c){ return std::toupper(c); });
    return upperStr;
}

std::string Utils::join(const std::vector<std::string>& vec, const std::string& delimiter) {
    if (vec.empty()) {
        return "";
    }
    std::string result = vec[0];
    for (size_t i = 1; i < vec.size(); ++i) {
        result += delimiter;
        result += vec[i];
    }
    return result;
}

std::vector<std::string> Utils::parseStringToList(const std::string& str) {
    std::vector<std::string> list;
    if (str.empty()) return list;

    std::stringstream ss(str);
    std::string segment;
    while(std::getline(ss, segment, ',')) {
        std::string trimmedSegment = Utils::trimmed(segment);
        if (!trimmedSegment.empty()) {
            list.push_back(trimmedSegment);
        }
    }
    return list;
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
    int currentYear = currentTime->tm_year + 2000;
    int lowerYearLimit = currentYear - 200;
    if (date.month < 1 || date.month > 12){
        throw std::invalid_argument("Invalid month: " + date.toString());
    }

    try {
        int maxDay = Date::getDaysInMonth(date.month, date.year);
        if (date.day < 1 || date.day > maxDay){
             throw std::invalid_argument("Invalid day " + std::to_string(date.day) + " for month " + std::to_string(date.month) + ". Date: " + date.toString());
        }
    } catch (const std::out_of_range& e) {
        throw std::invalid_argument(std::string(e.what()) + ". Date: " + date.toString());
    }


    if (date.year < lowerYearLimit || date.year > currentYear){
        throw std::invalid_argument("Invalid year " + std::to_string(date.year) + ", out of range (" + std::to_string(lowerYearLimit) + "-" + std::to_string(currentYear) + "). Date: " + date.toString());
    }
}

void Utils::validPhoneNumber(const std::string& phoneNumber) {
    if (phoneNumber.empty() || phoneNumber == "None") {
        return;
    }
    if (phoneNumber.length() < 10 || phoneNumber.length() > 11) {
        throw std::invalid_argument("Invalid phonenumber size, must be 10 or 11 digits");
    }
    for (char c : phoneNumber) {
        if (!std::isdigit(c)) {
            throw std::invalid_argument("Phone number must contain only digits");
        }
    }
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
        try {
            nlohmann::json j = Utils::readJsonFromFile(Config::SPECIALIZATION_PATH);
            if (j.contains("specializations") && j["specializations"].is_array()) {
                for (const auto& spec : j["specializations"]) {
                    set.insert(Utils::toUpper(Utils::trimmed(spec.get<std::string>())));
                }
            }
            std::cout << "[INFO] Loaded " << set.size() << " specializations." << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Cannot load specializations: " << e.what() << std::endl;
            set = {"Cardiology", "Neurology", "Pediatrics"};
        }
        return set;
    }();
    
    if (specializationTable.find(Utils::toUpper(Utils::trimmed(specialization_))) == specializationTable.end()){
        throw std::invalid_argument("Specialization: " + specialization_ + " is not valid.");
    }
}

void Utils::validBloodType(const std::string &bloodType_) {
    static std::unordered_set<std::string> validTypes = [](){
        std::unordered_set<std::string> set;
        try {
            nlohmann::json j = Utils::readJsonFromFile(Config::BLOOD_TYPE_PATH);
            
            if (j.contains("bloodTypes") && j["bloodTypes"].is_array()) {
                for (const auto& bloodType : j["bloodTypes"]) {
                    std::string bt = bloodType.get<std::string>();
                    set.insert(Utils::toUpper(Utils::trimmed(bt)));
                }
            }
            
            std::cout << "[INFO] Loaded " << set.size() << " blood types from JSON." << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "[ERROR] Cannot load blood types: " << e.what() << std::endl;
            set = {"A+", "A-", "B+", "B-", "AB+", "AB-", "O+", "O-"};
            std::cerr << "[FALLBACK] Using default blood types." << std::endl;
        }
        return set;
    }();

    if (validTypes.find(Utils::toUpper(Utils::trimmed(bloodType_))) == validTypes.end()) {
        throw std::invalid_argument("BloodType: " + bloodType_ + " is not valid.");
    }
}

void Utils::validUserName(const std::string &username_) {
    std::string trimmedUsername = Utils::trimmed(username_);
    
    if (trimmedUsername.empty()) {
        throw std::invalid_argument("Username must not be empty or only whitespace.");
    }
    
    if (trimmedUsername.size() < 4 || trimmedUsername.size() > 20) {
        throw std::invalid_argument("Username: " + trimmedUsername + " is not valid. Must be 4-20 characters.");
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
        while (std::getline(file, line)) set.insert(Utils::trimmed(line));
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
    std::string e = Utils::trimmed(email);
    const std::regex pattern(
        R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"
    );
    if (!std::regex_match(e, pattern)) {
        throw std::invalid_argument("Invalid email format: " + email);
    }
}

void Utils::validCCCD(const std::string& CCCD) {
    if (CCCD.size() != 12){
        throw std::invalid_argument("Invalid CCCD format: " + CCCD);
    }
}


void Utils::writeJsonToFile(const std::string& filePath, const nlohmann::json& j){
    std::string fullPath = filePath;
#ifdef DATA_DIR
    if (!filePath.empty() && filePath[0] != '/' && (filePath.size() < 2 || filePath[1] != ':')) {
        fullPath = std::string(DATA_DIR) + "/" + filePath;
    }
#endif
    std::ofstream outFile(fullPath);
    if (!outFile.is_open()) {
        throw std::runtime_error("Error: Could not open file \"" + fullPath + "\" for writing.");
    }
    outFile << j.dump(4);
    outFile.flush();
    if (outFile.fail()) {
        throw std::runtime_error("Error: Failed to write to file \"" + fullPath + "\".");
    }
    outFile.close();
    std::cout << "[SUCCESS] Data saved to: " << fullPath << std::endl;
}

nlohmann::json Utils::readJsonFromFile(const std::string& filePath){
    std::string fullPath = filePath;
#ifdef DATA_DIR
    if (!filePath.empty() && filePath[0] != '/' && (filePath.size() < 2 || filePath[1] != ':')) {
        fullPath = std::string(DATA_DIR) + "/" + filePath;
    }
#endif
    std::ifstream inFile(fullPath);
    if (!inFile.is_open()) {
        std::cerr << "Error: Could not open file \"" << fullPath << "\" for reading.\n";
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
    std::stringstream ss(text);
    std::string line;
    while (std::getline(ss, line)) {
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

std::unordered_map<std::string, double> Utils::loadMedicinePrices() {
    std::unordered_map<std::string, double> prices;
    
    try {
        nlohmann::json j = readJsonFromFile(Config::MEDICINE_PRICE_PATH);
        
        if (j.contains("medicines") && j["medicines"].is_array()) {
            for (const auto& med : j["medicines"]) {
                std::string name = med.value("name", "");
                double price = med.value("price", 0.0);
                if (!name.empty() && price > 0) {
                    prices[name] = price;
                }
            }
        }
        
        qDebug() << "[INFO] Loaded" << prices.size() << "medicine prices from JSON";
        
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Cannot load medicine prices:" << e.what();
        prices["Paracetamol"] = 5000;
        prices["Ibuprofen"] = 8000;
        prices["Amoxicillin"] = 12000;
    }
    
    return prices;
}

std::vector<std::pair<std::string, double>> Utils::getAllMedicinesWithPrices() {
    std::vector<std::pair<std::string, double>> result;
    
    try {
        nlohmann::json j = readJsonFromFile(Config::MEDICINE_PRICE_PATH);
        
        if (j.contains("medicines") && j["medicines"].is_array()) {
            for (const auto& med : j["medicines"]) {
                std::string name = med.value("name", "");
                double price = med.value("price", 0.0);
                if (!name.empty()) {
                    result.push_back({name, price});
                }
            }
        }
        
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Cannot get medicine list:" << e.what();
    }
    
    return result;
}

std::vector<nlohmann::json> Utils::searchMedicines(const std::string& searchText) {
    std::vector<nlohmann::json> results;
    
    try {
        nlohmann::json j = readJsonFromFile(Config::MEDICINE_PRICE_PATH);
        
        if (!j.contains("medicines") || !j["medicines"].is_array()) {
            return results;
        }
        
        std::string lowerSearch = toLower(searchText);
        
        for (const auto& med : j["medicines"]) {
            std::string name = toLower(med.value("name", ""));
            std::string category = toLower(med.value("category", ""));
            std::string description = toLower(med.value("description", ""));
            
            if (name.find(lowerSearch) != std::string::npos ||
                category.find(lowerSearch) != std::string::npos ||
                description.find(lowerSearch) != std::string::npos) {
                results.push_back(med);
            }
        }
        
    } catch (const std::exception& e) {
        qWarning() << "[ERROR] Cannot search medicines:" << e.what();
    }
    
    return results;
}
