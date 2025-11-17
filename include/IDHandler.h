#pragma once

#include <unordered_set>
#include <stdexcept>

template<typename Entity>
class IDHandler {
    private:
        static std::unordered_set<size_t> IDTable;
        static size_t lastID;

    public:
        static size_t generateID();
        static bool checkDuplicate(size_t ID);
        static void registerID(size_t ID);
        static void unregisterID(size_t ID);
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
bool IDHandler<Entity>::checkDuplicate(size_t ID){
    if (IDTable.find(ID) == IDTable.end()) {
        return false;
    }
    return true;
}

template<typename Entity>
void IDHandler<Entity>::registerID(size_t ID){
    if (checkDuplicate(ID)){
        throw std::invalid_argument("ID already exist.");
    }
    IDTable.insert(ID);
}

template<typename Entity>
void IDHandler<Entity>::unregisterID(size_t ID){
    IDTable.erase(ID);
}