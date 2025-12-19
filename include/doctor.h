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

    // Returns a vector of all days in the schedule
    std::vector<std::string> getDays() const {
        std::vector<std::string> days;
        for (const auto& [day, _] : schedule) {
            days.push_back(day);
        }
        return days;
    }

    std::vector<std::tuple<std::string, std::string, bool>> getIntervalsWithBooking(
        const std::string& day,
        const std::vector<std::pair<std::string, std::string>>& bookedSlots
    ) const {
        std::vector<std::tuple<std::string, std::string, bool>> intervals;
        auto it = schedule.find(day);
        if (it == schedule.end()) return intervals;

        for (const auto& [start, end] : it->second) {
            int startHour = std::stoi(start.substr(0,2));
            int endHour = std::stoi(end.substr(0,2));
            for (int h = startHour; h < endHour; ++h) {
                std::string intervalStart = (h < 10 ? "0" : "") + std::to_string(h) + ":00";
                std::string intervalEnd = (h+1 < 10 ? "0" : "") + std::to_string(h+1) + ":00";
                bool isBooked = false;
                for (const auto& [bookedStart, bookedEnd] : bookedSlots) {
                    if (!(bookedEnd <= intervalStart || bookedStart >= intervalEnd)) {
                        isBooked = true;
                        break;
                    }
                }
                intervals.emplace_back(intervalStart, intervalEnd, isBooked);
            }
        }
        return intervals;
    }

    std::vector<std::pair<std::string, std::string>> getAvailableTimeSlotsForDay(const std::string& day, const std::vector<std::pair<std::string, std::string>>& bookedSlots) const {
        std::vector<std::pair<std::string, std::string>> availableSlots;
        auto it = schedule.find(day);
        if (it == schedule.end()) return availableSlots;
        // Sort booked slots by start time
        std::vector<std::pair<std::string, std::string>> sortedBooked = bookedSlots;
        std::sort(sortedBooked.begin(), sortedBooked.end());

        for (const auto& [schedStart, schedEnd] : it->second) {
            std::string currentStart = schedStart;
            for (const auto& [bookedStart, bookedEnd] : sortedBooked) {
                if (bookedStart > currentStart && bookedStart < schedEnd) {
                    availableSlots.emplace_back(currentStart, bookedStart);
                    currentStart = bookedEnd;
                } else if (bookedStart <= currentStart && bookedEnd > currentStart) {
                    currentStart = bookedEnd;
                }
            }
            if (currentStart < schedEnd) {
                availableSlots.emplace_back(currentStart, schedEnd);
            }
        }
        return availableSlots;
    }

    std::vector<std::pair<std::string, std::string>> getBookedIntervals(
        const std::string& day,
        const std::vector<std::pair<std::string, std::string>>& bookedSlots
    ) const {
        std::vector<std::pair<std::string, std::string>> bookedIntervals;
        auto it = schedule.find(day);
        if (it == schedule.end()) return bookedIntervals;

        for (const auto& [schedStart, schedEnd] : it->second) {
            for (const auto& [bookedStart, bookedEnd] : bookedSlots) {
                // Check for overlap
                if (!(bookedEnd <= schedStart || bookedStart >= schedEnd)) {
                    // Compute the overlapping interval
                    std::string overlapStart = std::max(schedStart, bookedStart);
                    std::string overlapEnd = std::min(schedEnd, bookedEnd);
                    if (overlapStart < overlapEnd) {
                        bookedIntervals.emplace_back(overlapStart, overlapEnd);
                    }
                }
            }
        }
        return bookedIntervals;
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
        std::string description;
    
    public:
        Doctor();

        Doctor(const std::string& name_, char gender_, const std::string& birthday_, const std::string &phoneNumber_, const std::string &CCCD_, const std::string &email_, 
                const std::string& specialization_, const std::string& doctorStatus_,
                const WorkingSchedule& workingSchedule_, const std::string& room_ = "", const std::string& description_ = "");

        ~Doctor();

        Doctor(const Doctor& other);
        Doctor& operator=(const Doctor& other);
        Doctor(Doctor&& other) noexcept;
        Doctor& operator=(Doctor&& other) noexcept;

        void setRoom(const std::string &room_);
        void setSpecialization(const std::string &specialization_);
        void setStatus(const std::string& doctorStatus_);
        void setWorkingSchedule(const WorkingSchedule& schedule_);
        void setDescription(const std::string& description_);

        void addPatientID(int ID_);
        void removePatientID(int ID_);

        std::string getSpecialization() const {return specialization;}
        Status getStatus() const {return doctorStatus;}
        const std::unordered_set<int>& getPatientIDs() const {return patientIDs;}
        std::string getInfo() const override;
        std::string getRoom() const;
        const WorkingSchedule& getWorkingSchedule() const {return workingSchedule;}
        std::string getDescription() const {return description;}

        // convertor
        static std::string statusToString(Status status);
        static Doctor::Status statusFromString(const std::string& str);
        nlohmann::json toJson() const override;
        void fromJson(const nlohmann::json &j) override;
};        
