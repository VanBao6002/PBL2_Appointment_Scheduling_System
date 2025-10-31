#pragma once
#include <iostream>

class ISerializable {
public:
    virtual bool loadFromStream(std::istream& is) = 0;
    virtual void saveToStream(std::ostream& os) const = 0;
    virtual ~ISerializable() = default;
};