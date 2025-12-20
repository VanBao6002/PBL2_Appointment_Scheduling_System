#include <utility>
#include "doctor.h"

Doctor::Doctor() : Person(), specialization(""), patientIDs(), doctorStatus(Doctor::Status::Active), room(""), description("") { }

Doctor::Doctor(const std::string& name_, char gender_, const std::string& birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_, 
                const std::string& specialization_, const std::string& doctorStatus_,
                const WorkingSchedule& workingSchedule_, const std::string& room_, const std::string& description_)
    : Person(name_, gender_, birthday_, phoneNumber_, CCCD_, email_) {

    setSpecialization(specialization_);
    setStatus(doctorStatus_);
    setWorkingSchedule(workingSchedule_);
    setRoom(room_);
    setDescription(description_);

    int ID = static_cast<int>(IDHandler<Doctor>::generateID());
    setID(ID);
    IDHandler<Doctor>::registerID(ID);
}

Doctor::~Doctor(){}

// Copy Constructor - Only copy ID, do not generate new one
Doctor::Doctor(const Doctor& other)
        : Person(other),
            specialization(other.specialization),
            patientIDs(other.patientIDs),
            doctorStatus(other.doctorStatus),
            workingSchedule(other.workingSchedule),
            room(other.room),
            description(other.description)
{
    ID = other.ID;
}

// Copy Assignment Operator - Safe ID handling, do not generate new ID
Doctor& Doctor::operator=(const Doctor& other)
{
    if (this != &other) {
        Person::operator=(other);
        specialization = other.specialization;
        patientIDs = other.patientIDs;
        doctorStatus = other.doctorStatus;
        workingSchedule = other.workingSchedule;
        room = other.room;
        description = other.description;
        ID = other.ID;
    }
    return *this;
}

// Move Constructor - Move ID, do not generate new one
Doctor::Doctor(Doctor&& other) noexcept
        : Person(std::move(other)),
            specialization(std::move(other.specialization)),
            patientIDs(std::move(other.patientIDs)),
            doctorStatus(other.doctorStatus),
            workingSchedule(std::move(other.workingSchedule)),
            room(std::move(other.room)),
            description(std::move(other.description))
{
    ID = other.ID;
    other.ID = 0;
}

// Move Assignment Operator - Move ID safely, do not generate new one
Doctor& Doctor::operator=(Doctor&& other) noexcept
{
    if (this != &other) {
        Person::operator=(std::move(other));
        specialization = std::move(other.specialization);
        patientIDs = std::move(other.patientIDs);
        doctorStatus = other.doctorStatus;
        workingSchedule = std::move(other.workingSchedule);
        room = std::move(other.room);
        description = std::move(other.description);
        ID = other.ID;
        other.ID = 0;
    }
    return *this;
}

void Doctor::setSpecialization(const std::string &specialization_){
    Utils::validSpecialization(Utils::trimmed(specialization_));
    specialization = Utils::trimmed(specialization_);
}

void Doctor::setStatus(const std::string& doctorStatus_){
    doctorStatus = statusFromString(Utils::trimmed(doctorStatus_));
}

void Doctor::setWorkingSchedule(const WorkingSchedule& schedule) {
    workingSchedule = schedule;
}

void Doctor::setRoom(const std::string &room_) {
    room = Utils::trimmed(room_);
}

void Doctor::setDescription(const std::string &description_) {
    description = Utils::trimmed(description_);
}

void Doctor::addPatientID(int ID_) {
    if (patientIDs.count(ID_)) {
        throw std::invalid_argument("Patient ID already exists in doctor's list.");
    }
    patientIDs.insert(ID_);
}

void Doctor::removePatientID(int ID_) {
    if (!patientIDs.count(ID_)) {
        throw std::invalid_argument("Patient ID not found in doctor's list.");
    }
    patientIDs.erase(ID_);
}

std::string Doctor::getInfo() const{
    std::string info = "Doctor Info:\n";
    info += "ID: " + std::to_string(getID()) + "\n";
    info += "Name: " + getName() + "\n";
    info += "Gender: " + std::string(1, getGender()) + "\n";
    info += "Birthday: " + getBirthday().toString() + "\n"; 
    info += "Phone Number: " + phoneNumber + "\n";
    info += "Specialization: " + specialization + "\n";
    info += "Room: " + room + "\n";
    info += "Description: " + description + "\n";
    info += "Status: ";
    info += statusToString(doctorStatus) + "\n";
    info += "Email: " + email + "\n";
    info += "Patient IDs: ";
    for (const auto& pid : patientIDs) {
        info += std::to_string(pid) + " ";
    }
    info += "\n";
    return info;
}

std::string Doctor::getRoom() const {
    return room;
}

std::string Doctor::statusToString(Doctor::Status status) {
    switch (status) {
        case Doctor::Status::Active: return "Active";
        case Doctor::Status::OnLeave: return "OnLeave";
        case Doctor::Status::Retired: return "Retired";
        default: return "Unknown";
    }
}

Doctor::Status Doctor::statusFromString (const std::string& str){
    std::string lowerStr = Utils::toLower(str);
    if (lowerStr == "active") return Doctor::Status::Active; 
    if (lowerStr == "onleave") return Doctor::Status::OnLeave;
    if (lowerStr == "retired") return Doctor::Status::Retired;
    throw std::invalid_argument("Unknown status: " + str);
}

nlohmann::json Doctor::toJson() const {
    nlohmann::json j;
    j["ID"] = ID;
    j["name"] = name;
    j["gender"] = std::string(1, gender);
    j["birthday"] = birthday.toJson();
    j["phoneNumber"] = phoneNumber; 
    j["CCCD"] = CCCD;
    j["email"] = email;
    j["specialization"] = specialization;
    j["room"] = room;
    j["description"] = description;
    nlohmann::json patientIDsJson = nlohmann::json::array();
    for (const auto& pid : patientIDs) {
        patientIDsJson.push_back(pid);
    }
    j["patientIDs"] = patientIDsJson;
    j["doctorStatus"] = statusToString(doctorStatus);
    auto wsJson = workingSchedule.toJson();
    j["workingSchedule"] = wsJson.is_null() ? nlohmann::json::object() : wsJson;
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
    if (j.contains("CCCD")) CCCD = j.at("CCCD").get<std::string>();
    if (j.contains("email")) email = j.at("email").get<std::string>();
    if (j.contains("specialization")) {
        specialization = j.at("specialization").get<std::string>();
    }
    if (j.contains("room")) {
        room = j.at("room").get<std::string>();
    }
    if (j.contains("description")) {
        description = j.at("description").get<std::string>();
    }
    if (j.contains("patientIDs")) {
        patientIDs.clear();
        for (const auto& pid : j.at("patientIDs")) {
            patientIDs.insert(pid.get<int>());
        }
    }
    if (j.contains("doctorStatus")) {
        doctorStatus = statusFromString(j.at("doctorStatus").get<std::string>());
    }
    if (j.contains("workingSchedule")) {
        workingSchedule.fromJson(j.at("workingSchedule"));
    }
}
