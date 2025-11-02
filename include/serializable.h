#pragma once
#include <iostream>

struct ISerializable {
    virtual void serialize(std::ofstream& ofs) const = 0;
    virtual void deserialize(std::ifstream& ifs) = 0;
    virtual ~ISerializable() = default;
};