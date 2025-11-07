#pragma once
#include "person.h"
#include "medicalRecord.h"
#include "prescription.h"
#include <string>
#include "json.hpp"  // nlohmann/json

class Patient : public Person {
private:
    std::string bloodType;
    std::string nameMother;
    std::string nameFather;
    MedicalRecord medicalRecord;
    Prescription prescription;

public:
    Patient();
    Patient(const std::string &name_, char gender_, const Date &birthday_, int ID_,
            const std::string &bloodType_, const std::string &nameMother_, const std::string &nameFather_);

    virtual ~Patient() = default;
    virtual std::string getInfo() const override;

    std::string getBloodType() const { return bloodType; }
    std::string getNameMother() const { return nameMother; }
    std::string getNameFather() const { return nameFather; }

    void setBloodType(const std::string &bloodType_);
    void setNameMother(const std::string &nameMother_);
    void setNameFather(const std::string &nameFather_);

    // JSON
    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json &j);
};
