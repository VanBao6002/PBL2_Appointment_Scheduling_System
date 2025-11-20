#include "doctor.h"


Doctor::Doctor() : Person(), specialization(""), patientIDs(), doctorStatus(Status::Available){
    int ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
}
Doctor::Doctor(const std::string& name_, char gender_, const std::string& birthday_, const std::string &phoneNumber_, const std::string& email_, const std::string& specialization_, const std::string& doctorStatus_) : Person(name_, gender_, birthday_, phoneNumber_) {

    setEmail(email_);
    setSpecialization(specialization_);
    setStatus(doctorStatus_);

    int ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
} 

void Doctor::setSpecialization(const std::string &specialization_){
    Utils::validSpecialization(Utils::trimmed(specialization_));
    specialization = Utils::trimmed(specialization_);
}

void Doctor::setStatus(const std::string& doctorStatus_){
    doctorStatus = statusFromString(Utils::trimmed(doctorStatus_));
}

void Doctor::setEmail(const std::string &email_){
    Utils::validName(Utils::trimmed(email_));
    email = Utils::trimmed(email_);
}

// Add patient ID
void Doctor::addPatientID(int ID_) {
    if (std::find(patientIDs.begin(), patientIDs.end(), ID_) != patientIDs.end()) {
        throw std::invalid_argument("Patient ID already exists in doctor's list.");
    }
    patientIDs.push_back(ID_);
}

void Doctor::removePatientID(int patientID_) {
    auto it = std::find(patientIDs.begin(), patientIDs.end(), patientID_);
    if (it == patientIDs.end()) {
        throw std::invalid_argument("Patient ID not found in doctor's list.");
    }
    patientIDs.erase(it);
}


std::string Doctor::getInfo() const{
    std::string info = "Doctor Info:\n";
    info += "ID: " + std::to_string(getID()) + "\n";
    info += "Name: " + getName() + "\n";
    info += "Gender: " + std::string(1, getGender()) + "\n";
    info += "Birthday: " + getBirthday().toString() + "\n"; 
    info += "Phone Number: " + phoneNumber + "\n";
    info += "Specialization: " + specialization + "\n";
    info += "Status: ";
    std::string statusStr = (doctorStatus == Doctor::Status::Available) ? "Available" : "Unavailable";
    info += (doctorStatus == Doctor::Status::Available) ? "Available\n" : "Unavailable\n";
    info += "Email: " + email + "\n";
    info += "Patient IDs: ";
    for (const auto& pid : patientIDs) {
        info += std::to_string(pid) + " ";
    }
    info += "\n";
    return info;
}


Doctor::Status Doctor::statusFromString (const std::string& str){
    if (Utils::toLower(str) == "available") return Doctor::Status::Available;
    if (Utils::toLower(str) == "unavailable") return Doctor::Status::Unavailable;
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
    j["phoneNumber"] = phoneNumber;
    j["email"] = email;
    j["specialization"] = specialization;
    nlohmann::json patientIDsJson;
    for (const auto& pid : patientIDs) {
        patientIDsJson.push_back(pid);
    }
    j["patientIDs"] = patientIDsJson;
    j["doctorStatus"] = (doctorStatus == Status::Available) ? "Available" : "Unavailable";
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
    if (j.contains("phoneNumber")) phoneNumber = j.at("phoneNumber").get<std::string>();
    if (j.contains("email")) email = j.at("email").get<std::string>();
    if (j.contains("specialization")) specialization = j.at("specialization").get<std::string>();
    if (j.contains("patientIDs")) {
        patientIDs.clear();
        for (const auto& pid : j.at("patientIDs")) {
            patientIDs.push_back(pid.get<int>());
        }
    }
    if (j.contains("doctorStatus")) doctorStatus = statusFromString(j.at("doctorStatus").get<std::string>());
    
}