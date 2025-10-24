#pragma once

#include "person.h"
#include "medicalRecord.h"
#include "prescription.h"
#include <string>
class Patient : public Person{
    private:
        std::string bloodType;
        std::string nameMother;
        std::string nameFather;
<<<<<<< HEAD
        MedicalRecord medicalRecord;
        Prescription prescription;
=======
        
>>>>>>> b59bb7934cffd7e9b48d2daecb93496762ab1ac5
    public:
        Patient();

        Patient(const std::string &name_, char gender_, const Date &birthday_, int ID_, const std::string &bloodType, const std::string &nameMother,const std::string &nameFather);
        
        virtual ~Patient() = default;

        virtual std::string getInfo() const override;

        //getters
        std::string getBloodType() const {return bloodType;}
        std::string getNameMother() const {return nameMother;}
        std::string getNameFather() const {return nameFather;}

        //setters
        void setBloodType(const std::string &bloodType_);
        void setNameMother(const std::string &nameMother_);
        void setNameFather(const std::string &nameFather_);
};

