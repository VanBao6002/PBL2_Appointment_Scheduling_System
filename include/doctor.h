#pragma once

#include "person.h"
#include "patient.h"
#include "serializable.h"
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>

class Doctor : public Person, public ISerializable {
    public:
        enum class Status {Unavailable, Available};

    private:
        std::string specialization; // Chuyên Khoa
        std::unordered_map<int, Patient> patientIDs; // ID bệnh nhân được bác sĩ đảm nhận
        Status doctorStatus;
    
    public:
        Doctor();

        Doctor(int ID_, const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, Status doctorStatus_);

        virtual ~Doctor() = default;

        void setSpecialization(const std::string &specialization_);
        void setStatus(Status doctorStatus_);

        void addPatientID(const Patient &patientID_);
        void removePatientID(int patientID_);

        std::string getSpecialization() const {return specialization;}
        Status getStatus() const {return doctorStatus;}
        const std::unordered_map<int, Patient>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;

        void serialize(std::ostream& os) const override;
        void deserialize(std::istream& is) override;
};        
