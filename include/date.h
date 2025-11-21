#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#include "json.hpp"

struct Date {
    int day;
    int month;
    int year;

public:
    Date() : day(1), month(1), year(2000) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {
    }

    int getDay() const { return day; }
    int getMonth() const { return month; }
    int getYear() const { return year; }

    // Chuyển đối tượng Date thành chuỗi định dạng "DD/MM/YYYY"
    std::string toString() const {
        char buffer[11]; // "DD/MM/YYYY" + null terminator
        // Đã sửa lỗi: tham số thứ ba phải là year
        std::snprintf(buffer, sizeof(buffer), "%02d/%02d/%04d", day, month, year);
        return std::string(buffer);
    }

    // Tạo đối tượng Date từ chuỗi định dạng "DD/MM/YYYY"
    static Date fromString(const std::string& str) {
        int d,m,y;
        char sep1, sep2;
        std::istringstream iss(str);
        iss >> d >> sep1 >> m >> sep2 >> y;
        if (iss.fail() || sep1 != '/' || sep2 != '/') {
            throw std::runtime_error("Invalid date format. Expected DD/MM/YYYY");
        }
        return Date(d, m , y);
    }
    
    nlohmann::json toJson() const {
        nlohmann::json j;
        j["day"] = day;
        j["month"] = month;
        j["year"] = year;
        return j;
    }

    static Date fromJson(const nlohmann::json& j) {
        int d = j.value("day", 1);
        int m = j.value("month", 1);
        int y = j.value("year", 2000);
        return Date(d, m, y);
    }

    void addDays(int days) {
        while (days > 0) {
            int daysInCurrentMonth = getDaysInMonth(month, year);
            if (day + days <= daysInCurrentMonth) {
                day += days;
                days = 0;
            } else {
                days -= (daysInCurrentMonth - day + 1);
                day = 1;
                month++;
                if (month > 12) {
                    month = 1;
                    year++;
                }
            }
        }
    }

    // Overloaded Operators for I/O (Sử dụng định dạng D M Y)
    friend std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << d.day << ' ' << d.month << ' ' << d.year;
        return os;
    }

    friend std::istream& operator>>(std::istream& is, Date& d) {
        is >> d.day >> d.month >> d.year;
        return is;
    }

    bool operator>(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }

    bool operator<(const Date& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }

    bool operator<=(const Date& other) const {
        return *this < other || *this == other;
    }

    bool operator>=(const Date& other) const {
        return *this > other || *this == other;
    }

    bool operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }
    
    bool operator!=(const Date& other) const {
        return !(*this == other);
    }

    static int getDaysInMonth(int month, int year) {
        static const int daysPerMonth[] = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        if (month == 2 && isLeapYear(year)) return 29;
        if (month < 1 || month > 12) throw std::out_of_range("Invalid month value");
        return daysPerMonth[month - 1];
    }
    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
};
inline void to_json(nlohmann::json& j, const Date& d) {
    j = {
        {"day", d.day},
        {"month", d.month},
        {"year", d.year}
    };
}

inline void from_json(const nlohmann::json& j, Date& d) {
    d.day = j.value("day", 1);
    d.month = j.value("month", 1);
    d.year = j.value("year", 2000);
}