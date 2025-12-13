#pragma once

#include <unordered_set>
#include <stdexcept>

template<typename Entity>
class IDHandler {
    private:
        static std::unordered_set<size_t> IDTable;
        static std::unordered_set<std::string> CCCDTable;
        static size_t lastID;

    public:     
        static size_t generateID();
        static bool checkDuplicateID(size_t ID);
        static void registerID(size_t ID);
        static void unregisterID(size_t ID);
        static void setCurrentID(size_t ID);
        static void resetIDTable(); 

        static void registerCCCD(const std::string& CCCD);
        static void unregisterCCCD(const std::string& CCCD);
        static bool checkDuplicateCCCD(const std::string& CCCD);
        static void resetCCCDTable();
};

template<typename Entity>
std::unordered_set<size_t> IDHandler<Entity>::IDTable;

template<typename Entity>
size_t IDHandler<Entity>::lastID = 1;

template<typename Entity>
size_t IDHandler<Entity>::generateID(){
    return lastID++;
}

template<typename Entity>
bool IDHandler<Entity>::checkDuplicateID(size_t ID){
    if (IDTable.find(ID) == IDTable.end()) {
        return false;
    }
    return true;
}

template<typename Entity>
void IDHandler<Entity>::registerID(size_t ID){
    if (checkDuplicateID(ID)){
        throw std::invalid_argument("ID already exist.");
    }
    IDTable.insert(ID);
}

template<typename Entity>
void IDHandler<Entity>::unregisterID(size_t ID){
    IDTable.erase(ID);
}

template <typename Entity>
void IDHandler<Entity>::setCurrentID(size_t ID) {
    IDHandler<Entity>::lastID = ID + 1; 
}

template <typename Entity>
void IDHandler<Entity>::resetIDTable() {
    IDTable.clear();
    lastID = 1;
}


template<typename Entity>
std::unordered_set<std::string> IDHandler<Entity>::CCCDTable;

template <typename Entity>
void IDHandler<Entity>::registerCCCD(const std::string& CCCD) {
    if (checkDuplicateCCCD(CCCD)){
        throw std::invalid_argument("CCCD already exist.");
    }
    CCCDTable.insert(CCCD);
}

template <typename Entity>
void IDHandler<Entity>::unregisterCCCD(const std::string& CCCD){
    CCCDTable.erase(CCCD);
}

template <typename Entity>
bool IDHandler<Entity>::checkDuplicateCCCD(const std::string& CCCD){
    if (CCCDTable.find(CCCD) == CCCDTable.end()) {
        return false;
    }
    return true;
}

template <typename Entity>
void IDHandler<Entity>::resetCCCDTable(){
    CCCDTable.clear();
}