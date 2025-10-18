#include <iostream>
#include <cassert>
#include "userManager.h"
#include "user.h"

int main() {
    UserManager um;
    User u;
    u.setID(1);
    u.setUsername("testuser");
    u.setRole(User::Role::ADMIN);
    u.setPassword("password123");

    // Test addUser
    um.addUser(1, u);
    assert(um.getUserByID(1).getUsername() == "testuser");

    // Test editUser
    User u2 = u;
    u2.setUsername("newuser");
    um.editUser(1, u2);
    assert(um.getUserByID(1).getUsername() == "newuser");

    // Test changeRole
    um.changeRole(1, User::Role::DOCTOR);
    assert(um.getUserByID(1).getRole() == User::Role::DOCTOR);

    std::cout << "All UserManager tests passed!" << std::endl;
    return 0;
}