
#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "person.h"
#include "medicalRecord.h"
#include "prescription.h"
#include "json.hpp"
#include "IDHandler.h"
#include "utils.h"
#include "date.h"

class Patient : public Person {
private:
    std::string bloodType;
    std::vector<std::string> allergies; // dị ứng
    std::vector<std::string> chronicDiseases; // bệnh nền
    std::string nameMother;
    std::string nameFather;
    std::vector<int> medicalRecordIDs; // hồ sơ bệnh án 

public:
    Patient();
    Patient(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_,  
            const std::string &bloodType_,const std::string &allergies_, const std::string &chronicDisease_, const std::string &nameMother_, const std::string &nameFather_);

    ~Patient() = default;

    std::string getInfo() const override;
    std::string getBloodType() const { return bloodType; }
    const std::vector<std::string>& getAllergies() const { return allergies; }
    const std::vector<std::string>& getChronicDiseases() const { return chronicDiseases; }
    std::string getNameMother() const { return nameMother; }
    std::string getNameFather() const { return nameFather; }
    const std::vector<int>& getMedicalRecords() const { return medicalRecordIDs; }

    void addMedicalRecord(int recordID);
    void setBloodType(const std::string &bloodType_);
    void setAllergies(const std::string &allergies_);
    void setChronicDiseases(const std::string &chronicDieases_);
    void setNameMother(const std::string &nameMother_);
    void setNameFather(const std::string &nameFather_);

    // JSON
    nlohmann::json toJson() const override;
    void fromJson(const nlohmann::json &j) override;
};
