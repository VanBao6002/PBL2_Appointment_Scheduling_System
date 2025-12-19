#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "json.hpp"
#include "doctor.h"


struct TimeInterval {
    std::string start;
    std::string end;
    bool isBooked;

    TimeInterval(const std::string& start_, const std::string& end_, bool isBooked_ = false)
        : start(start_), end(end_), isBooked(isBooked_) {}

    void setBooked(bool isBooked_) { isBooked = isBooked_; }

    static std::vector<TimeInterval> getIntervalsWithBooking(
        const std::string& day, const WorkingSchedule& schedule, const std::vector<std::pair<std::string, std::string>>& bookedSlots
    ) {
        std::vector<TimeInterval> intervals;
        auto workingTime = schedule.getWorkingTime(day);
        if (workingTime.empty()) return intervals;

        for (const auto& [start, end] : workingTime) {
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

    static std::vector<std::pair<std::string, std::string>> getAvailableTimeSlotsForDay(
        const std::string& day, const WorkingSchedule& schedule, const std::vector<std::pair<std::string, std::string>>& bookedSlots
    ) {
        std::vector<std::pair<std::string, std::string>> availableSlots;
        auto workingTime = schedule.getWorkingTime(day);
        if (workingTime.empty()) return availableSlots;

        std::vector<std::pair<std::string, std::string>> sortedBooked = bookedSlots;
        std::sort(sortedBooked.begin(), sortedBooked.end());

        for (const auto& [schedStart, schedEnd] : workingTime) {
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

    static std::vector<std::pair<std::string, std::string>> getBookedIntervals(
        const std::string& day, const WorkingSchedule& schedule, const std::vector<std::pair<std::string, std::string>>& bookedSlots
    ) {
        std::vector<std::pair<std::string, std::string>> bookedIntervals;
        auto workingTime = schedule.getWorkingTime(day);
        if (workingTime.empty()) return bookedIntervals;

        for (const auto& [schedStart, schedEnd] : workingTime) {
            for (const auto& [bookedStart, bookedEnd] : bookedSlots) {
                if (!(bookedEnd <= schedStart || bookedStart >= schedEnd)) {
                    std::string overlapStart = std::max<std::string>(schedStart, bookedStart);
                    std::string overlapEnd = std::min<std::string>(schedEnd, bookedEnd);
                    if (overlapStart < overlapEnd) {
                        bookedIntervals.emplace_back(overlapStart, overlapEnd);
                    }
                }
            }
        }
        return bookedIntervals;
    }
};