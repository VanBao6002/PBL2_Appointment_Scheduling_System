#pragma once
#include <string>
class User {
    public: 
        enum class Role {ADMIN, DOCTOR, PATIENT};

    private:
        enum Role {ADMIN, DOCTOR, PATIENT};
        Role userRole;
        std::string userName;
        std::string passwordHash;
        std::string permission;

    public:
        User() = default;
    


};