#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <sstream>
#include <limits>
#include <algorithm>
#include <vector>

#include "person.h"
#include "patient.h"


struct WorkingSchedule {
    // "Day" / "StartTime" / "EndTime"
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::string>>> schedule;

    nlohmann::json toJson() const {
        nlohmann::json j;
        for (const auto& [day, times] : schedule) {
            nlohmann::json timesJson = nlohmann::json::array();
            for (const auto& [start, end] : times) {
                timesJson.push_back({{"StartTime", start}, {"EndTime", end}});
            }
            j[day] = timesJson;
        }
        return j;
    }

    void fromJson(const nlohmann::json& j) {
        schedule.clear();
        for (auto it = j.begin(); it != j.end(); ++it) {
            std::string day = it.key();
            std::vector<std::pair<std::string, std::string>> times;
            for (const auto& timeObj : it.value()) {
                std::string start = timeObj.at("StartTime").get<std::string>();
                std::string end = timeObj.at("EndTime").get<std::string>();
                times.emplace_back(start, end);
            }
            schedule[day] = times;
        }
    }
};

class Doctor : public Person{
    public:
        enum class Status {Active, OnLeave, Retired};

    private:
        std::string specialization; // Chuyên Khoa
        std::unordered_set<int> patientIDs; // ID bệnh nhân được bác sĩ đảm nhận
        Status doctorStatus;
        std::string room;
        WorkingSchedule workingSchedule;
    
    public:
        Doctor();

        Doctor(const std::string& name_, char gender_, const std::string& birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_, 
                const std::string& specialization_, const std::string& doctorStatus_,
                const WorkingSchedule& workingSchedule_, const std::string& room_ = "");

        ~Doctor();

        Doctor(const Doctor& other);
        Doctor& operator=(const Doctor& other);
        Doctor(Doctor&& other) noexcept;
        Doctor& operator=(Doctor&& other) noexcept;

        void setRoom(const std::string &room_);
        void setSpecialization(const std::string &specialization_);
        void setStatus(const std::string& doctorStatus_);
        void setWorkingSchedule(const WorkingSchedule& schedule_);

        void addPatientID(int ID_);
        void removePatientID(int ID_);

        std::string getSpecialization() const {return specialization;}
        Status getStatus() const {return doctorStatus;}
        const std::unordered_set<int>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;
        std::string getRoom() const;
        const WorkingSchedule& getWorkingSchedule() const {return workingSchedule;}

        // convertor
        static std::string statusToString(Status status);
        static Doctor::Status statusFromString(const std::string& str);
        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &j) override;
};        
