#include <iostream>
#include <fstream>
#include "../include/json.hpp"
#include "../include/patient.h"
#include "../include/date.h"

int main() {
    // 1️⃣ Tạo patient
    Patient p1;

    // 2️⃣ Ghi ra file JSON
    std::ofstream outFile("patient.json");
    if (!outFile) {
        std::cerr << "Không thể mở file để ghi!\n";
        return -1;
    }

    nlohmann::json j = p1.toJson();
    outFile << j.dump(4); // dump(4) để format đẹp
    outFile.close();
    std::cout << "Đã ghi patient.json\n";

    // 3️⃣ Đọc lại file JSON
    std::ifstream inFile("patient.json");
    if (!inFile) {
        std::cerr << "Không thể mở file để đọc!\n";
        return -1;
    }

    nlohmann::json j2;
    inFile >> j2;
    inFile.close();

    Patient p2;
    p2.fromJson(j2);

    // 4️⃣ Hiển thị thông tin đọc lại
    std::cout << "Patient đọc lại:\n";
    std::cout << "ID: " << p2.getID() << "\n";
    std::cout << "Name: " << p2.getName() << "\n";
    std::cout << "Gender: " << p2.getGender() << "\n";
    std::cout << "Birthday: " << p2.getBirthday().getDay() << "/"
              << p2.getBirthday().getMonth() << "/" << p2.getBirthday().getYear() << "\n";
    std::cout << "Blood Type: " << p2.getBloodType() << "\n";
    std::cout << "Mother's Name: " << p2.getNameMother() << "\n";
    std::cout << "Father's Name: " << p2.getNameFather() << "\n";

    return 0;
}
