#include "patient.h"

Patient::Patient(): Person(), bloodType("AB+"), allergies{"none"}, chronicDiseases{"none"}, nameMother("NGUYEN VAN B"), nameFather("NGUYEN VAN A"), medicalRecords() {
    int ID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(ID);
}

Patient::Patient(const std::string &name_, char gender_, const Date &birthday_, const std::string &bloodType_, const std::string &allergies_, const std::string &chronicDisease_, const std::string &nameMother_, const std::string &nameFather_) : Person(name_, gender_, birthday_) {

    setBloodType(bloodType_);
    setAllergies(allergies_);
    setChronicDiseases(chronicDisease_);
    setNameMother(nameMother_);
    setNameFather(nameFather_);

    int ID = static_cast<int>(IDHandler<Patient>::generateID());
    setID(ID);
} 

void Patient::addMedicalRecord(const MedicalRecord& record) {
    Utils::validMedicalRecord(record);
    medicalRecords.push_back(record);
}   

void Patient::setBloodType(const std::string &bloodType_) {
    std::string trimmedBloodType = Utils::trimmed(bloodType_);
    Utils::validBloodType(trimmedBloodType);
    bloodType = trimmedBloodType;
}

void Patient::setAllergies(const std::string &allergies_) {
    std::string trimmedAllergy = Utils::trimmed(allergies_);
    Utils::validName(trimmedAllergy);
    allergies.push_back(trimmedAllergy);
}

void Patient::setChronicDiseases(const std::string &chronicDiseases_) {
    std::string trimmedDisease = Utils::trimmed(chronicDiseases_);
    Utils::validName(trimmedDisease);
    chronicDiseases.push_back(trimmedDisease);
}

void Patient::setNameMother(const std::string &nameMother_) {
    std::string trimmedName = Utils::trimmed(nameMother_);
    Utils::validName(trimmedName);
    nameMother = trimmedName;
}

void Patient::setNameFather(const std::string &nameFather_) {
    std::string trimmedName = Utils::trimmed(nameFather_);
    Utils::validName(trimmedName);
    nameFather = trimmedName;
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
    j["medicalRecords"] = nlohmann::json::array();
    for (const auto& record : medicalRecords) {
        j["medicalRecords"].push_back(record.toJson());
    }
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
    if (j.contains("medicalRecords")) {
        medicalRecords.clear();
        for (const auto& recJson : j.at("medicalRecords")) {
            MedicalRecord rec;
            rec.fromJson(recJson);
            medicalRecords.push_back(rec);
        }
    }
}
