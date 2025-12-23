#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "person.h"
#include "medicalRecord.h"
#include "prescription.h"
#include "json.hpp"
#include "IDHandler.h"
#include "utils.h"
#include "date.h"

class Patient : public Person {
private:
    std::string insuranceID;
    std::string bloodType;
    std::vector<std::string> allergies;
    std::vector<std::string> chronicDiseases;
    std::string nameMother;
    std::string nameFather;
    std::string phoneMother;
    std::string phoneFather;
    std::vector<int> medicalRecordIDs;

public:
    Patient();
    Patient(const std::string &name_, char gender_, const std::string &birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_,
            const std::string &insuranceID_,
            const std::string &bloodType_, const std::string &allergies_, 
            const std::string &chronicDisease_, 
            const std::string &nameMother_, const std::string &nameFather_,
            const std::string &phoneMother_, const std::string &phoneFather_);

    Patient(const Patient& other);
    Patient& operator=(const Patient& other);
    Patient(Patient&& other) noexcept;
    Patient& operator=(Patient&& other) noexcept;

    ~Patient();

    std::string getInfo() const override;
    
    std::string getInsuranceID() const { return insuranceID; }
    void setInsuranceID(const std::string &insuranceID_);
    std::string getBloodType() const { return bloodType; }
    const std::vector<std::string>& getAllergies() const { return allergies; }
    const std::vector<std::string>& getChronicDiseases() const { return chronicDiseases; }
    std::string getNameMother() const { return nameMother; }
    std::string getNameFather() const { return nameFather; }
    std::string getPhoneMother() const { return phoneMother; }
    std::string getPhoneFather() const { return phoneFather; }
    const std::vector<int>& getMedicalRecordIDs() const { return medicalRecordIDs; }

    void addMedicalRecord(int recordID);
    void setMedicalRecordIDs(const std::vector<int>& recordIDs);
    void setBloodType(const std::string &bloodType_);
    void setAllergies(const std::string &allergies_);
    void setChronicDiseases(const std::string &chronicDieases_);
    void setNameMother(const std::string &nameMother_);
    void setNameFather(const std::string &nameFather_);
    void setPhoneMother(const std::string &phoneMother_);
    void setPhoneFather(const std::string &phoneFather_);

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json &j);
};

inline void to_json(nlohmann::json& j, const Patient& p) {
    j = p.toJson();
}

inline void from_json(const nlohmann::json& j, Patient& p) {
    p.fromJson(j);
}