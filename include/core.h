#pragma once
#include <vector>
#include <string>
#include <QStringList>
#include <unordered_set>
class Core {  
    public:
        static void loadAll();
        static void saveAll();
        static std::unordered_set<std::string> loadSpecializations();
        static std::unordered_set<std::string> loadRooms();
};