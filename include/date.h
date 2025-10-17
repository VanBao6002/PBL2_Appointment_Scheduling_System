#pragma once

struct Date{
    int day;
    int month;
    int year;

    std::string toString(){
        char buffer[17]; // "YYYY-MM-DD HH:MM" + null terminator
        std::snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", year, month, day);
        return std::string(buffer);
    }
};
