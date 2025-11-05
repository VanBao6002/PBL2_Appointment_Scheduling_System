#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
struct Date {
    int day;
    int month;
    int year;
    public:
    Date() : day(1), month(1), year(2000) {}
    
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    std::string toString() const {
        char buffer[17]; // "YYYY-MM-DD HH:MM" + null terminator
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
        return std::string(buffer);
    }

    static Date fromString(const std::string& str) {
        int y, m, d;
        char sep1, sep2;
        std::istringstream iss(str);
        iss >> y >> sep1 >> m >> sep2 >> d;
        if (sep1 != '-' || sep2 != '-') throw std::runtime_error("Invalid date format");
        return Date(y, m, d);
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

    bool operator==(const Date& other) const {
        return year == other.year && month == other.month && day == other.day;
    }

    static int getDaysInMonth(int month, int year) {
        static const int daysPerMonth[] = {
            31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
        };
        
        if (month == 2 && isLeapYear(year)) {
            return 29;
        }
        return daysPerMonth[month - 1];
    }

    static bool isLeapYear(int year) {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }
};
