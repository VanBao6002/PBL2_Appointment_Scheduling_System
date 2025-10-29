#pragma once

#include <string>
struct Date {
    int day;
    int month;
    int year;
    public:
    // Constructor mặc định
    Date() : day(1), month(1), year(2000) {}
    
    // Constructor với 3 tham số
    Date(int y, int m, int d) : year(y), month(m), day(d) {}

    std::string toString() const {
        char buffer[17]; // "YYYY-MM-DD HH:MM" + null terminator
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
        return std::string(buffer);
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

    // Toán tử so sánh
    bool operator>(const Date& other) const {
        if (year != other.year) return year > other.year;
        if (month != other.month) return month > other.month;
        return day > other.day;
    }

    // Hàm phụ trợ để tính số ngày trong tháng
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
