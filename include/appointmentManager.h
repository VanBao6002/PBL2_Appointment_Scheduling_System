#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>


#include "appointment.h"
#include "doctorManager.h" // Chắc chắn đã include
#include "patientManager.h" // Chắc chắn đã include
#include "userManager.h"    // Chắc chắn đã include
#include "Utils.h"          // Cần cho getIDLog và load/save toJson
#include "IDHandler.h"      // Cần cho IDHandler
#include "json.hpp"         // Cần cho toJson/fromJson

class AppointmentManager{
private:
    std::unordered_map<int, std::string> log;
    std::unordered_map<int, Appointment> appointmentTable;


    AppointmentManager() = default; // Constructor private
    AppointmentManager(const AppointmentManager&) = delete; // Xóa copy constructor
    AppointmentManager& operator=(const AppointmentManager&) = delete; // Xóa copy assignment operator

public:
    // Phương thức tĩnh để lấy instance duy nhất
    static AppointmentManager& getInstance() {
        static AppointmentManager instance; // Khởi tạo lần đầu tiên được gọi
        return instance;
    }
    // --- HẾT MỚI ---

    ~AppointmentManager() = default;

    void addAppointment(const Appointment &doc_);
    void editAppointment(int ID_, const Appointment &newAppointment);
    void removeAppointment(int ID_);

    const Appointment& getAppointmentByID(int ID_) const;
    // Chỉnh sửa: Trả về vector để dễ thao tác với QList và phân trang
    std::vector<Appointment> getAllAppointmentsAsVector() const; 
    const std::unordered_map<int, Appointment>& getAllAppointmentsMap() const; // Giữ lại nếu cần map
    
    const std::unordered_map<int, std::string>& getAllLog() const;
    const std::string& getIDLog(int ID_) const;

    // loader
    void loadFromFile(const std::string& path);
    void saveToFile(const std::string& path);
};