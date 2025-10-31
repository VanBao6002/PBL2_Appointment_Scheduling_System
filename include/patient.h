#pragma once

#include "person.h"
#include "medicalRecord.h"
#include "prescription.h"
#include "serializable.h"
#include <string>
#include <iostream>
#include <sstream>

class Patient : public Person, public ISerializable {
    private:
        std::string bloodType;
        std::string nameMother;
        std::string nameFather;
        MedicalRecord medicalRecord;
        Prescription prescription;
    public:
        Patient();

        Patient(const std::string &name_, char gender_, const Date &birthday_, int ID_, const std::string &bloodType, const std::string &nameMother,const std::string &nameFather);
        
        virtual ~Patient() = default;

        virtual std::string getInfo() const override;

        std::string getBloodType() const {return bloodType;}
        std::string getNameMother() const {return nameMother;}
        std::string getNameFather() const {return nameFather;}

        void setBloodType(const std::string &bloodType_);
        void setNameMother(const std::string &nameMother_);
        void setNameFather(const std::string &nameFather_);

        bool loadFromStream(std::istream& is);
        void saveToStream(std::ostream& os) const;
};

