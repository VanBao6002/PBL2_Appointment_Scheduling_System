#include "core.h"
#include "patientManager.h"
#include "doctorManager.h"
#include "appointmentManager.h"
#include "userManager.h"
#include "medicalRecordManager.h"
#include "config.h"


void Core::loadAll() {
    UserManager::getInstance().loadFromFile(Config::USER_PATH);
    DoctorManager::getInstance().loadFromFile(Config::DOCTOR_PATH);
    PatientManager::getInstance().loadFromFile(Config::PATIENT_PATH);
    MedicalRecordManager::getInstance().loadFromFile(Config::MEDICAL_RECORD_PATH);
    AppointmentManager::getInstance().loadFromFile(Config::APPOINTMENT_PATH);
}

void Core::saveAll() {
    DoctorManager::getInstance().saveToFile(Config::DOCTOR_PATH);
    PatientManager::getInstance().saveToFile(Config::PATIENT_PATH);
    MedicalRecordManager::getInstance().saveToFile(Config::MEDICAL_RECORD_PATH);
    AppointmentManager::getInstance().saveToFile(Config::APPOINTMENT_PATH);
    UserManager::getInstance().saveToFile(Config::USER_PATH);
}

std::unordered_set<std::string> Core::loadSpecializations(){
    std::unordered_set<std::string> specializationTable;
    try {
        nlohmann::json j = Utils::readJsonFromFile(Config::SPECIALIZATION_PATH);
        if (j.contains("specializations") && j["specializations"].is_array()) {
            for (const auto& spec : j["specializations"]) {
                specializationTable.insert(Utils::trimmed(spec.get<std::string>()));
            }
        }
        std::cout << "[INFO] Loaded " << specializationTable.size() << " specializations." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Cannot load specializations: " << e.what() << std::endl;
        // Fallback
        specializationTable = {"Da liễu", "Mắt", "Răng hàm mặt"};
    }
    return specializationTable;
}