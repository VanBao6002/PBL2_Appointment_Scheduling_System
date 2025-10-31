gui/
├── mainwindow/             # Di chuyển các file mainwindow hiện có vào đây
│   ├── mainwindow.h
│   ├── mainwindow.cpp
│   └── mainwindow.ui
├── patients/              # UI components cho quản lý bệnh nhân
│   ├── patientlist.h      
│   ├── patientlist.cpp    # Màn hình danh sách bệnh nhân
│   ├── patientlist.ui
│   ├── patientform.h      
│   ├── patientform.cpp    # Form thêm/sửa thông tin bệnh nhân
│   ├── patientform.ui
│   ├── patientdetail.h    
│   ├── patientdetail.cpp  # Màn hình xem chi tiết bệnh nhân
│   └── patientdetail.ui
├── doctors/              # UI components cho quản lý bác sĩ
│   ├── doctorlist.h
│   ├── doctorlist.cpp     # Màn hình danh sách bác sĩ
│   ├── doctorlist.ui
│   ├── doctorform.h
│   ├── doctorform.cpp     # Form thêm/sửa thông tin bác sĩ
│   ├── doctorform.ui
│   ├── doctordetail.h
│   ├── doctordetail.cpp   # Màn hình xem chi tiết bác sĩ
│   └── doctordetail.ui
├── appointments/         # UI components cho quản lý lịch hẹn
│   ├── appointmentlist.h
│   ├── appointmentlist.cpp # Màn hình danh sách lịch hẹn
│   ├── appointmentlist.ui
│   ├── appointmentform.h
│   ├── appointmentform.cpp # Form đặt lịch hẹn
│   ├── appointmentform.ui
│   ├── calendar.h
│   ├── calendar.cpp       # Màn hình lịch
│   └── calendar.ui
└── shared/              # Các resources và styles chung
    ├── styles.qss        # CSS cho UI
    ├── icons/           # Folder chứa icons
    │   ├── patient.png
    │   ├── doctor.png
    │   └── appointment.png
    └── resources.qrc    # File resource của Qt