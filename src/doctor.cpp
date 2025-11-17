#include "doctor.h"


Doctor::Doctor() : Person(), specialization(""), patientIDs(), doctorStatus(Status::Available){
    int ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
}
Doctor::Doctor(const std::string& name_, char gender_, const Date& birthday_, const std::string& specialization_, const std::string& doctorStatus_, const std::string& phoneNumber_, const std::string& email_) : Person(name_, gender_, birthday_) {

    setSpecialization(specialization_);
    setStatus(doctorStatus_);
    setPhoneNumber(phoneNumber_);
    setEmail(email_);

    int ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
} 

void Doctor::setSpecialization(const std::string &specialization_){
    std::string trimmedSpecialization = Utils::trimmed(specialization_);
    Utils::validSpecialization(trimmedSpecialization);
    specialization = trimmedSpecialization;
}

void Doctor::setStatus(const std::string& doctorStatus_){
    std::string trimmedStatus = Utils::trimmed(doctorStatus_);

    doctorStatus = statusFromString(trimmedStatus);
}

void Doctor::setPhoneNumber(const std::string &phonenumber_){
    std::string trimmedPhoneNumber = Utils::trimmed(phonenumber_);
    if (trimmedPhoneNumber.size() != 11) throw std::invalid_argument("Invalid phonenumber.");
    phoneNumber = trimmedPhoneNumber;
}

void Doctor::setEmail(const std::string &email_){
    std::string trimmedEmail = Utils::trimmed(email_);
    Utils::validName(trimmedEmail);
    email = trimmedEmail;
}

// Add patient ID
void Doctor::addPatientID(const Patient& patient) {
    patientIDs[patient.getID()] = patient;
}

void Doctor::removePatientID(int patientID_) {
    if (!IDHandler<Patient>::checkDuplicate(patientID_)){
        throw std::invalid_argument("Patient ID not found.");
    }
    patientIDs.erase(patientID_);
}


std::string Doctor::getInfo() const{
    std::string info = "Doctor Info:\n";
    info += "Name: " + getName() + "\n";
    info += "Gender: " + std::string(1, getGender()) + "\n";
    info += "Birthday: " + getBirthday().toString() + "\n"; 
    info += "ID: " + std::to_string(getID()) + "\n";
    info += "Specialization: " + specialization + "\n";
    info += "Status: ";
    std::string statusStr = (doctorStatus == Doctor::Status::Available) ? "Available" : "Unavailable";
    info += (doctorStatus == Doctor::Status::Available) ? "Available\n" : "Unavailable\n";
    info += "Patient IDs: ";
    for (const auto& [pid, patient] : patientIDs) {
        info += std::to_string(pid) + " (" + patient.getName() + ") ";
    }
    info += "\n";
    return info;
}


Doctor::Status Doctor::statusFromString(const std::string& str){
    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    if (lowerStr == "available") return Doctor::Status::Available;
    if (lowerStr == "unavailable") return Doctor::Status::Unavailable;
    throw std::invalid_argument("Unknown status: " + str);
}

nlohmann::json Doctor::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = {
        {"day", birthday.getDay()},
        {"month", birthday.getMonth()},
        {"year", birthday.getYear()}
    };
    j["specialization"] = specialization;
    nlohmann::json patientIDsJson;
    for (const auto& [pid, patient] : patientIDs) {
        patientIDsJson[std::to_string(pid)] = patient.toJson();
    }
    j["patientIDs"] = patientIDsJson;
    j["doctorStatus"] = doctorStatus;
    j["phoneNumber"] = phoneNumber;
    j["email"] = email;
    return j;
}

void Doctor::fromJson(const nlohmann::json &j) {
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
    if (j.contains("specialization")) specialization = j.at("specialization").get<std::string>();
    if (j.contains("doctorStatus")) doctorStatus = statusFromString(j.at("doctorStatus").get<std::string>());
    if (j.contains("phoneNumber")) phoneNumber = j.at("phoneNumber").get<std::string>();
    if (j.contains("email")) email = j.at("email").get<std::string>();
}