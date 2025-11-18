#include "patient.h"

Patient::Patient(): Person(), bloodType("AB+"), allergies{"none"}, chronicDiseases{"none"}, nameMother("NGUYEN VAN B"), nameFather("NGUYEN VAN A"), medicalRecordIDs() {
    int ID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(ID);
}

Patient::Patient(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_, const std::string &bloodType_, const std::string &allergies_, const std::string &chronicDisease_, const std::string &nameMother_, const std::string &nameFather_) : Person(name_, gender_, birthday_, phoneNumber_) {

    setBloodType(bloodType_);
    setAllergies(allergies_);
    setChronicDiseases(chronicDisease_);
    setNameMother(nameMother_);
    setNameFather(nameFather_);

    int ID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(ID);
} 

void Patient::addMedicalRecord(int recordID) {
    if (std::find(medicalRecordIDs.begin(), medicalRecordIDs.end(), recordID) != medicalRecordIDs.end()) {
        throw std::invalid_argument("Medical record ID already exists for this patient.");
    }
    medicalRecordIDs.push_back(recordID);
}   

void Patient::setBloodType(const std::string &bloodType_) {
    Utils::validBloodType(Utils::trimmed(bloodType_));
    bloodType = Utils::trimmed(bloodType_);
}

void Patient::setAllergies(const std::string &allergies_) {
    Utils::validName(Utils::trimmed(allergies_));
    allergies.push_back(Utils::trimmed(allergies_));
}

void Patient::setChronicDiseases(const std::string &chronicDiseases_) {
    Utils::validName(Utils::trimmed(chronicDiseases_));
    chronicDiseases.push_back(Utils::trimmed(chronicDiseases_));
}

void Patient::setNameMother(const std::string &nameMother_) {
    Utils::validName(Utils::trimmed(nameMother_));
    nameMother = Utils::trimmed(nameMother_);
}

void Patient::setNameFather(const std::string &nameFather_) {
    Utils::validName(Utils::trimmed(nameFather_));
    nameFather = Utils::trimmed(nameFather_);
}

std::string Patient::getInfo() const {
    std::string info = Person::getInfo();
    info += "\nBlood Type: " + bloodType;
    info += "\nMother's Name: " + nameMother;
    info += "\nFather's Name: " + nameFather;
    return info;
}

// ---------------- JSON -----------------
nlohmann::json Patient::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = {
        {"day", birthday.getDay()},
        {"month", birthday.getMonth()},
        {"year", birthday.getYear()}
    };
    j["bloodType"] = bloodType;
    j["nameMother"] = nameMother;
    j["nameFather"] = nameFather;
    j["allergies"] = allergies;
    j["chronicDiseases"] = chronicDiseases;
    // Serialize medicalRecords as array of json objects
    nlohmann::json medicalRecordIDsJson;
    for (const auto& ID : medicalRecordIDs) {
        medicalRecordIDsJson.push_back(ID);
    }
    j["medicalRecordIDs"] = medicalRecordIDsJson;
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
    if (j.contains("bloodType")) bloodType = j.at("bloodType").get<std::string>();
    if (j.contains("nameMother")) nameMother = j.at("nameMother").get<std::string>();
    if (j.contains("nameFather")) nameFather = j.at("nameFather").get<std::string>();
    if (j.contains("allergies")) allergies = j.at("allergies").get<std::vector<std::string>>();
    if (j.contains("chronicDiseases")) chronicDiseases = j.at("chronicDiseases").get<std::vector<std::string>>();
    medicalRecordIDs.clear();
    if (j.contains("medicalRecordIDs")) {
        for (const auto& id : j.at("medicalRecordIDs")) {
            medicalRecordIDs.push_back(id.get<int>());
        }
    }
}
