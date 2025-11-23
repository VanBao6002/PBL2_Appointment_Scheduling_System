#include "patient.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>



Patient::Patient(): Person(), bloodType("AB+"), allergies{}, chronicDiseases{}, nameMother("NGUYEN VAN B"), nameFather("NGUYEN VAN A"), medicalRecordIDs() {
}

Patient::Patient(const std::string &name_, char gender_, const std::string &birthday_, 
                 const std::string &phoneNumber_, const std::string &bloodType_, 
                 const std::string &allergies_, const std::string &chronicDisease_, 
                 const std::string &nameMother_, const std::string &nameFather_) 
    : Person(name_, gender_, birthday_, phoneNumber_) {
    
    setBloodType(bloodType_);
    setAllergies(allergies_);
    setChronicDiseases(chronicDisease_);
    setNameMother(nameMother_);
    setNameFather(nameFather_);
    
    int newID = static_cast<int>(IDHandler<Patient>::generateID());
    ID = newID;
    IDHandler<Patient>::registerID(static_cast<size_t>(newID));
}

// Copy Constructor - CHỈ COPY ID, không tạo mới
Patient::Patient(const Patient& other) 
    : Person(other) {
    bloodType = other.bloodType;
    allergies = other.allergies;
    chronicDiseases = other.chronicDiseases;
    nameMother = other.nameMother;
    nameFather = other.nameFather;
    medicalRecordIDs = other.medicalRecordIDs;
    ID = other.ID;
}

// Assignment Operator - Xử lý ID an toàn, ko tạo mới ID
Patient& Patient::operator=(const Patient& other) {
    if (this != &other) {
        // Copy dữ liệu từ Person
        Person::operator=(other);
        
        bloodType = other.bloodType;
        allergies = other.allergies;
        chronicDiseases = other.chronicDiseases;
        nameMother = other.nameMother;
        nameFather = other.nameFather;
        medicalRecordIDs = other.medicalRecordIDs;
        
        ID = other.ID;
    }
    return *this;
}

// Move Constructor - Di chuyển ID ko tạo mới 
Patient::Patient(Patient&& other) noexcept 
    : Person(std::move(other)) {
    bloodType = std::move(other.bloodType);
    allergies = std::move(other.allergies);
    chronicDiseases = std::move(other.chronicDiseases);
    nameMother = std::move(other.nameMother);
    nameFather = std::move(other.nameFather);
    medicalRecordIDs = std::move(other.medicalRecordIDs);
    ID = other.ID;
    other.ID = 0;
}

// Move Assignment Operator - Di chuyển ID an toàn, ko tạo mới
Patient& Patient::operator=(Patient&& other) noexcept {
    if (this != &other) {
       // Move dữ liệu từ Person
        Person::operator=(std::move(other));
        
        bloodType = std::move(other.bloodType);
        allergies = std::move(other.allergies);
        chronicDiseases = std::move(other.chronicDiseases);
        nameMother = std::move(other.nameMother);
        nameFather = std::move(other.nameFather);
        medicalRecordIDs = std::move(other.medicalRecordIDs);
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

Patient::~Patient(){
    if (ID > 0) {
        try {
            IDHandler<Patient>::unregisterID(ID);
        } catch (...) {}
    }
}

void Patient::addMedicalRecord(int recordID) {
    if (std::find(medicalRecordIDs.begin(), medicalRecordIDs.end(), recordID) != medicalRecordIDs.end()) {
        throw std::invalid_argument("Medical record ID already exists for this patient.");
    }
    medicalRecordIDs.push_back(recordID);
}

std::string Patient::getInfo() const {
    return "Patient ID: " + std::to_string(ID) + 
           ", Name: " + name + 
           ", Gender: " + std::string(1, gender) +
           ", Blood Type: " + bloodType +
           ", Mother: " + nameMother + 
           ", Father: " + nameFather;
}

void Patient::setBloodType(const std::string &bloodType_) {
    Utils::validBloodType(bloodType_);
    bloodType = Utils::toUpper(Utils::trimmed(bloodType_));
}

void Patient::setAllergies(const std::string &allergies_) {
    allergies = Utils::parseStringToList(Utils::trimmed(allergies_));
}

void Patient::setChronicDiseases(const std::string &chronicDiseases_) {
    chronicDiseases = Utils::parseStringToList(Utils::trimmed(chronicDiseases_));
}

void Patient::setNameMother(const std::string &nameMother_) {
    Utils::validName(Utils::trimmed(nameMother_));
    nameMother = Utils::trimmed(nameMother_);
}

void Patient::setNameFather(const std::string &nameFather_) {
    Utils::validName(Utils::trimmed(nameFather_));
    nameFather = Utils::trimmed(nameFather_);
}

nlohmann::json Patient::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = birthday.toJson();
    j["phoneNumber"] = phoneNumber; 
    j["bloodType"] = bloodType;
    j["nameMother"] = nameMother;
    j["nameFather"] = nameFather;
    j["allergies"] = allergies;
    j["chronicDiseases"] = chronicDiseases;
    j["medicalRecordIDs"] = medicalRecordIDs;
    std::cout << "[DEBUG] Patient toJson() - ID: " << ID << ", Name: " << name << std::endl;
    return j;
}

void Patient::fromJson(const nlohmann::json &j) {
    if (j.contains("ID")) ID = j.at("ID").get<int>();
    if (j.contains("name")) name = j.at("name").get<std::string>();
    if (j.contains("gender")) {
        std::string g = j.at("gender").get<std::string>();
        if (!g.empty()) gender = g[0];
    }
    if (j.contains("birthday")) {
        auto bd = j.at("birthday");
        int d = bd.value("day", 1);
        int m = bd.value("month", 1);
        int y = bd.value("year", 2000);
        birthday = Date(d, m, y);
    }
    if (j.contains("phoneNumber")) phoneNumber = j.at("phoneNumber").get<std::string>();
    if (j.contains("bloodType")) bloodType = j.at("bloodType").get<std::string>();
    if (j.contains("nameMother")) nameMother = j.at("nameMother").get<std::string>();
    if (j.contains("nameFather")) nameFather = j.at("nameFather").get<std::string>();
    if (j.contains("allergies") && j.at("allergies").is_array()) {
        allergies = j.at("allergies").get<std::vector<std::string>>();
    } else {
        allergies.clear();
    }
    if (j.contains("chronicDiseases") && j.at("chronicDiseases").is_array()) {
        chronicDiseases = j.at("chronicDiseases").get<std::vector<std::string>>();
    } else {
        chronicDiseases.clear();
    }
    
    if (j.contains("medicalRecordIDs") && j.at("medicalRecordIDs").is_array()) {
        medicalRecordIDs = j.at("medicalRecordIDs").get<std::vector<int>>();
    } else if (j.contains("medicalRecords") && j.at("medicalRecords").is_array()) {
        medicalRecordIDs.clear();
        for (const auto& record : j.at("medicalRecords")) {
            if (record.contains("recordID") && record.at("recordID").is_number_integer()) {
                medicalRecordIDs.push_back(record.at("recordID").get<int>());
            }
        }
    } else {
        medicalRecordIDs.clear();
    }
}