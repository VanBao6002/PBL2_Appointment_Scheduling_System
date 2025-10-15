Hệ thống Quản lý Bệnh nhân (PMS) cần có những hành vi gì?

Hãy hình dung PMS giống như một phần mềm trợ lý cho bệnh viện/phòng khám. Nó phải quản lý con người (bệnh nhân, bác sĩ) và mối quan hệ giữa họ (lịch hẹn/khám).

0. Init
- Lớp Person
- Lớp Patient
- Lớp PatientManager
- Lớp Doctor
- Lớp DoctorManager
- Lớp Appointment
- Lớp AppointmentManager
- Lớp Utils
- Lớp MedicalRecord (Hồ sơ bệnh án)
- Lớp Prescription (đơn thuốc)
- Lớp User và Role (liên quan đến bảo mật & phân quyền)

1. Lớp Person
- Thuộc tính:
    + name , gender, birthday, ID
- Phương thức:
    + setter (hàm nhập):
        + setName, setGender, setBirthday, setID
    + getter (hàm xuất):
        + getName, getGender, getBirthday, getID
    * phương thức trừu tượng displayInfo

2.0 Lớp Patient
- Thuộc tính:
    + Kế thừa từ lớp Person;
    + bloodType, nameMother, nameFather
- Phương thức:
    + setter (hàm nhập):
        + setBloodType, setNameMother, setNameFather
    + getter (hàm xuất):
        + getBloodType, getNameMother, getNameFather

2.1. Lớp PatientManager 
- Thêm bệnh nhân mới (Mã ID, họ tên, tuổi, giới tính, thông tin liên hệ, tiền sử bệnh…). 

- Xem thông tin bệnh nhân theo ID.

- Sửa thông tin bệnh nhân (ví dụ: đổi số điện thoại, địa chỉ).

- Xóa bệnh nhân (nếu không còn hoạt động).

- Liệt kê tất cả bệnh nhân.

3.0 Lớp Doctor(Thừa kế từ lớp Person)
- Thuộc tính:
    + specialization (chuyên khoa)
    + patientsID (Vector chứa ID bệnh nhân đảm nhận)
- Phương thức:
    + getSpecialization (xuất tên của chuyên khoa)
    + addPatientID (thêm ID của bệnh nhân vào vector ID bệnh nhân)
    + removePatientID (xóa ID của bệnh nhân vào vector ID bệnh nhân)

3.1. Lớp DoctorManager
- Thêm bác sĩ (ID, họ tên, chuyên khoa, lịch trực/khả năng khám).

- Xem thông tin bác sĩ.

- Sửa thông tin bác sĩ.

- Xóa bác sĩ.

- Liệt kê tất cả bác sĩ.

- Kiểm tra thông tin bệnh nhân được bác sĩ đảm nhận

4.0 Lớp Appointment
- Trạng thái lịch hẹn

- Ngày giờ lịch hẹn 

- Phòng hẹn

4.1. Lớp AppointmentManager
- Đặt lịch hẹn (gồm bệnh nhân + bác sĩ + ngày/giờ + lý do khám).

- Xem chi tiết một lịch hẹn theo ID.

- Liệt kê tất cả lịch hẹn (có thể sắp xếp theo ngày hoặc theo bệnh nhân).

- Tìm lịch hẹn theo bác sĩ hoặc bệnh nhân.

- Hủy lịch hẹn.

- (Nâng cao) Kiểm tra xung đột thời gian khi đặt lịch.

5. MedicalRecord (Hồ sơ bệnh án)

- Lưu trữ chi tiết lịch sử khám chữa bệnh: ngày khám, chẩn đoán, điều trị, kết quả xét nghiệm.

- Gắn với Patient.

6. Prescription (Đơn thuốc)

- Tách riêng đơn thuốc thay vì để lẫn trong Patient.

- Thuộc về một MedicalRecord, chứa danh sách thuốc, liều lượng, hướng dẫn.

7. User và Role (liên quan đến bảo mật & phân quyền)

- User: Tài khoản đăng nhập (username, password).

- Role: Phân quyền (Admin, Doctor, Nurse, Patient).

- Điều này giúp tách biệt thông tin cá nhân (Person) và tài khoản hệ thống (User).

5. Tìm kiếm & Truy vấn

- Tìm bệnh nhân theo ID hoặc tên.

- Tìm bác sĩ theo ID hoặc chuyên khoa.

- Tìm lịch hẹn theo ngày, bệnh nhân hoặc bác sĩ.

6. Xử lý Dữ liệu

- Lưu danh sách bệnh nhân, bác sĩ, lịch hẹn ra file (patients.txt, doctors.txt, appointments.txt).

- Nạp dữ liệu từ file khi khởi động chương trình.

- (Nâng cao) Kết nối cơ sở dữ liệu (SQLite/MySQL).

7. Tính năng bổ sung (Nâng cao, Phase 4+)

- Gán bác sĩ chính cho mỗi bệnh nhân.

- Lưu lịch sử khám bệnh của bệnh nhân (các lần khám, chẩn đoán).

- Xuất báo cáo (ví dụ: tất cả bệnh nhân của bác sĩ X trong tuần này).

- Đăng nhập (quản trị viên / bác sĩ).

- Nhắc nhở lịch hẹn.