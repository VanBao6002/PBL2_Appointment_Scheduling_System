#pragma once

#include "date.h"
#include "doctor.h"
#include "doctorManager.h"
#include "patient.h"
#include "patientManager.h"
#include "user.h"
#include "medicalRecord.h"
#include "prescription.h"
#include <string>
#include <stdexcept>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <concepts>
#include <type_traits>
#include <fstream>

class Utils {
public:
    static bool isExpired(const Date& prescriptionDate, int duration);

    static int calculateTotalDays();
    static double calculateTotalCost();
    static std::string getDateTime();
    static std::string hashFunc(const std::string&);
    static std::string generatePrescriptionText(
        int prescriptionID, 
        const Date& prescriptionDate,
        int patientID, 
        int doctorID, 
        const std::string& diagnosis,
        const std::vector<std::pair<std::string, std::pair<int, int>>>& medicines,
        const std::string& additionalNotes = "");

    static void validName(const std::string &name);
    static void validGender(char gender);
    static void validDate(const Date &date_);
    static void validID(int ID);
    static void validTime(const std::string &time);

    static void validPatientID(const std::unordered_map<int, Patient> &patientIDs, int patientID_);
    static void validDoctorID(const std::unordered_map<int, Doctor> &doctorsList_, int ID_);
    static void validUserID(const std::unordered_map<int, User> &userTable_, int ID_);
    static void validSpecialization(const std::string &specialization_);
    static void validBloodType(const std::string &bloodType_);
    static void validUserName(const std::string &username_);
    static void validPassword(const std::string &password_);
    static void validRoom(const std::string &room_);
    static void validPrescription(const Prescription &prescription_);
    static void validMedicalRecord(const MedicalRecord &medicalrecord_);
    
    template<typename T>
        requires std::is_base_of_v<ISerializable, T>
    static void saveToFile(const std::string& filename, const std::unordered_map<int, T>& data) {
        std::ofstream ofs(filename, std::ios::binary);
        for (const auto& [id, obj] : data) {
            obj.serialize(ofs);
        }
    }

    template<typename T>
        requires std::is_base_of_v<ISerializable, T>
    static void loadFromFile(const std::string& filename, std::unordered_map<int, T>& data) {
        std::ifstream ifs(filename, std::ios::binary);
        while (ifs.peek() != EOF) {
            T obj;
            obj.deserialize(ifs);
            // Giả sử obj có member 'id'
            data[obj.id] = obj;
        }
    }

    template<typename T>
    static bool loadFromStream(std::istream &is, std::unordered_map<int, T> &dataMap) {
        requires std::is_base_of_v<ISerializable, T> 
        {
            T temp;
            std::string line;
            dataMap.clear();
            while (std::getline(is, line)) {
                if (line.empty() || line[0] == '#') continue; // Bỏ qua comment và dòng trống
                std::istringstream iss(line);
                if (temp.loadFromStream(iss)) {
                    dataMap[temp.getID()] = temp;
                }
            }
            return !dataMap.empty(); // Trả về true nếu có ít nhất 1 object được load
        }
    }

    template<typename T>
    static void saveToStream(std::ostream &os, const std::unordered_map<int, T> &dataMap) {
        requires std::is_base_of_v<ISerializable, T> 
        {
            os << "# Auto-generated stream - " << Utils::getDateTime() << "\n";
            os << "# Format: <Class-specific format>\n\n";
            for (const auto& pair : dataMap) {
                const T& obj = pair.second;
                obj.saveToStream(os);
                os << "\n";
            }
        }
    }
};

