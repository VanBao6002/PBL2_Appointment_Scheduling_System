#include <iostream>
#include "date.h"
#include "prescription.h"

int main() {
    // Test Date
    Date d1(2023, 11, 6);
    d1.addDays(1);
    std::cout << "Date after adding 1 day: " << d1.toString() << std::endl;

    // Test Prescription
    Prescription p(1, 100, 200);
    p.setDiagnosis("Flu");
    p.setAdditionalNotes("Patient should rest.");
    p.setPrescriptionDate(Date(2023, 11, 6));
    // You may need to adjust this if Medicine definition requires more fields
    p.addMedicine("Paracetamol", "500mg", 3, 5, "After meal");

    std::cout << "Prescription ID: " << p.getPrescriptionID() << std::endl;
    std::cout << "Diagnosis: " << p.getDiagnosis() << std::endl;
    std::cout << "Notes: " << p.getAdditionalNotes() << std::endl;
    std::cout << "Prescription Date: " << p.getPrescriptionDate().toString() << std::endl;

    const auto& meds = p.getMedicines();
    for (const auto& med : meds) {
        std::cout << "Medicine: " << med.name << ", Dosage: " << med.dosage << std::endl;
    }

    return 0;
}