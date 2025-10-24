#pragma once

#include <string>
struct Date{
    int day;
    int month;
    int year;

    std::string toString() const{
        char buffer[17]; // "YYYY-MM-DD HH:MM" + null terminator
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
        return std::string(buffer);
    }
};
