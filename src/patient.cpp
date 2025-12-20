#include "patient.h"
#include <algorithm>
#include <stdexcept>
#include <sstream>



Patient::Patient(): Person(), insuranceID(""), bloodType("AB+"), allergies{}, chronicDiseases{}, 
                   nameMother("NGUYEN VAN B"), nameFather("NGUYEN VAN A"), 
                   phoneMother(""), phoneFather(""), medicalRecordIDs() {  // Khởi tạo mặc định
}

Patient::Patient(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_,
            const std::string &insuranceID_,
            const std::string &bloodType_, const std::string &allergies_, 
            const std::string &chronicDisease_, 
            const std::string &nameMother_, const std::string &nameFather_,
            const std::string &phoneMother_, const std::string &phoneFather_)
    : Person(name_, gender_, birthday_, phoneNumber_, CCCD_, email_) {
    setInsuranceID(insuranceID_);
    setBloodType(bloodType_);
    setAllergies(allergies_);
    setChronicDiseases(chronicDisease_);
    setNameMother(nameMother_);
    setNameFather(nameFather_);
    setPhoneMother(phoneMother_);
    setPhoneFather(phoneFather_);

    int newID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(newID);
    IDHandler<Patient>::registerID(static_cast<size_t>(newID));
}

// Copy Constructor - CHỈ COPY ID, không tạo mới
Patient::Patient(const Patient& other) 
    : Person(other) {
    insuranceID = other.insuranceID;
    bloodType = other.bloodType;
    allergies = other.allergies;
    chronicDiseases = other.chronicDiseases;
    nameMother = other.nameMother;
    nameFather = other.nameFather;
    phoneMother = other.phoneMother;    // Copy số điện thoại mẹ
    phoneFather = other.phoneFather;    // Copy số điện thoại cha
    medicalRecordIDs = other.medicalRecordIDs;
    ID = other.ID;
}

// Assignment Operator - Xử lý ID an toàn, ko tạo mới ID
Patient& Patient::operator=(const Patient& other) {
    if (this != &other) {
        // Copy dữ liệu từ Person
        Person::operator=(other);
        insuranceID = other.insuranceID;
        bloodType = other.bloodType;
        allergies = other.allergies;
        chronicDiseases = other.chronicDiseases;
        nameMother = other.nameMother;
        nameFather = other.nameFather;
        phoneMother = other.phoneMother;    // Copy số điện thoại mẹ
        phoneFather = other.phoneFather;    // Copy số điện thoại cha
        medicalRecordIDs = other.medicalRecordIDs;
        ID = other.ID;
    }
    return *this;
}

// Move Constructor - Di chuyển ID ko tạo mới 
Patient::Patient(Patient&& other) noexcept 
    : Person(std::move(other)) {
    insuranceID = std::move(other.insuranceID);
    bloodType = std::move(other.bloodType);
    allergies = std::move(other.allergies);
    chronicDiseases = std::move(other.chronicDiseases);
    nameMother = std::move(other.nameMother);
    nameFather = std::move(other.nameFather);
    phoneMother = std::move(other.phoneMother);    // Move số điện thoại mẹ
    phoneFather = std::move(other.phoneFather);    // Move số điện thoại cha
    medicalRecordIDs = std::move(other.medicalRecordIDs);
    ID = other.ID;
    other.ID = 0;
}

// Move Assignment Operator - Di chuyển ID an toàn, ko tạo mới
Patient& Patient::operator=(Patient&& other) noexcept {
    if (this != &other) {
        // Move dữ liệu từ Person
        Person::operator=(std::move(other));
        insuranceID = std::move(other.insuranceID);
        bloodType = std::move(other.bloodType);
        allergies = std::move(other.allergies);
        chronicDiseases = std::move(other.chronicDiseases);
        nameMother = std::move(other.nameMother);
        nameFather = std::move(other.nameFather);
        phoneMother = std::move(other.phoneMother);    // Move số điện thoại mẹ
        phoneFather = std::move(other.phoneFather);    // Move số điện thoại cha
        medicalRecordIDs = std::move(other.medicalRecordIDs);
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

Patient::~Patient(){

}

void Patient::addMedicalRecord(int recordID) {
    if (std::find(medicalRecordIDs.begin(), medicalRecordIDs.end(), recordID) != medicalRecordIDs.end()) {
        throw std::invalid_argument("Medical record ID already exists for this patient.");
    }
    medicalRecordIDs.push_back(recordID);
}

std::string Patient::getInfo() const {
    std::ostringstream oss;
    oss << "Patient ID: " << ID
        << ", Name: " << name
        << ", Gender: " << gender
        << ", Birthday: " << birthday.toString()
        << ", Phone: " << phoneNumber
        << ", CCCD: " << CCCD
        << ", Email: " << email
        << ", InsuranceID: " << insuranceID
        << ", Blood Type: " << bloodType
        << ", Mother: " << nameMother << " (Phone: " << phoneMother << ")"
        << ", Father: " << nameFather << " (Phone: " << phoneFather << ")";

    if (!allergies.empty()) {
        oss << ", Allergies: ";
        for (size_t i = 0; i < allergies.size(); ++i) {
            oss << allergies[i];
            if (i != allergies.size() - 1) oss << "; ";
        }
    }

    if (!chronicDiseases.empty()) {
        oss << ", Chronic Diseases: ";
        for (size_t i = 0; i < chronicDiseases.size(); ++i) {
            oss << chronicDiseases[i];
            if (i != chronicDiseases.size() - 1) oss << "; ";
        }
    }

    if (!medicalRecordIDs.empty()) {
        oss << ", Medical Record IDs: ";
        for (size_t i = 0; i < medicalRecordIDs.size(); ++i) {
            oss << medicalRecordIDs[i];
            if (i != medicalRecordIDs.size() - 1) oss << ", ";
        }
    }

    return oss.str();
}

void Patient::setInsuranceID(const std::string &insuranceID_) {
    insuranceID = insuranceID_;
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
    if (nameMother_.empty()) {
        nameMother = "Trống";
        return;
    }
    Utils::validName(Utils::trimmed(nameMother_));
    nameMother = Utils::trimmed(nameMother_);
}

void Patient::setNameFather(const std::string &nameFather_) {
    if (nameFather_.empty()) {
        nameFather = "Trống";
        return;
    }
    Utils::validName(Utils::trimmed(nameFather_));
    nameFather = Utils::trimmed(nameFather_);
}

void Patient::setPhoneMother(const std::string &phoneMother_) {
    if (phoneMother_.empty()) {
        phoneMother = "Trống";
        return;
    }
    phoneMother = Utils::trimmed(phoneMother_);
}

void Patient::setPhoneFather(const std::string &phoneFather_) {
    if (phoneFather_.empty()) {
        phoneFather = "Trống";
        return;
    }
    phoneFather = Utils::trimmed(phoneFather_);
}

void Patient::setMedicalRecordIDs(const std::vector<int>& recordIDs) {
    medicalRecordIDs = recordIDs;
}

nlohmann::json Patient::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = birthday.toJson();
    j["phoneNumber"] = phoneNumber; 
    j["CCCD"] = CCCD;
    j["email"] = email;
    j["insuranceID"] = insuranceID;
    j["bloodType"] = bloodType;
    j["nameMother"] = nameMother;
    j["nameFather"] = nameFather;
    j["phoneMother"] = phoneMother;    // Thêm số điện thoại mẹ
    j["phoneFather"] = phoneFather;    // Thêm số điện thoại cha
    j["allergies"] = allergies;
    j["chronicDiseases"] = chronicDiseases;
    j["medicalRecordIDs"] = medicalRecordIDs;
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
    if (j.contains("CCCD")) CCCD = j.at("CCCD").get<std::string>();
    if (j.contains("email")) email = j.at("email").get<std::string>();
    if (j.contains("insuranceID")) insuranceID = j.at("insuranceID").get<std::string>();
    if (j.contains("bloodType")) bloodType = j.at("bloodType").get<std::string>();
    if (j.contains("nameMother")) nameMother = j.at("nameMother").get<std::string>();
    if (j.contains("nameFather")) nameFather = j.at("nameFather").get<std::string>();
    // Thêm đọc số điện thoại mẹ và cha (với giá trị mặc định nếu không có)
    if (j.contains("phoneMother")) phoneMother = j.at("phoneMother").get<std::string>();
    else phoneMother = "";
    if (j.contains("phoneFather")) phoneFather = j.at("phoneFather").get<std::string>();
    else phoneFather = "";
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