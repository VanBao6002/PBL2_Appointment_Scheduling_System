#ifndef ADMINWINDOW_H
#define ADMINWINDOW_H

#include <QMainWindow>
#include "addappointmentdialog.h"
#include "config.h"


namespace Ui {
class AdminWindow;
}

class AdminWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdminWindow(QWidget *parent = nullptr);
    ~AdminWindow();

private slots:
    // Slot cho Sidebar
    void on_appointmentManagerButton_clicked();
    void on_doctorManagerButton_clicked();
    void on_patientManagerButton_clicked();
    void on_userManagerButton_clicked();
    void on_medicalRecordButton_clicked();

    // Slot cho page_appointment
    void on_btnAddAppointment_clicked();
    void on_btnEditAppointment_clicked();
    void on_btnSearchAppointment_clicked();
    void on_btnRemoveAppointment_clicked();
    void on_btnNextPage_Appointment_clicked();
    void on_btnPrevPage_Appointment_clicked();
    void on_btnPage_Appointment_1_clicked();
    void on_btnPage_Appointment_2_clicked();
    void on_btnPage_Appointment_3_clicked();

    //slot cho page_Patient
    void on_btnAddPatient_clicked();
    void on_btnEditPatient_clicked();
    void on_btnRemovePatient_clicked();
    void on_btnSearchPatient_clicked();
    void on_btnSortAZPatient_clicked();
    void on_btnSortZAPatient_clicked();
    void on_btnViewPatientDetail_clicked();

    void on_btnNextPage_Patient_clicked();
    void on_btnPrevPage_Patient_clicked();
    void on_btnPage_Patient_1_clicked();
    void on_btnPage_Patient_2_clicked();
    void on_btnPage_Patient_3_clicked();

    //slot cho page_Doctor
    void on_btnAddDoctor_clicked();
    void on_btnEditDoctor_clicked();
    void on_btnRemoveDoctor_clicked();
    void on_btnSearchDoctor_clicked();
    void on_btnViewDoctorDetail_clicked();
    void on_btnSortAZDoctor_clicked();
    void on_btnSortZADoctor_clicked();

    void on_btnPrevPage_Doctor_clicked();
    void on_btnNextPage_Doctor_clicked();
    void on_btnPage_Doctor_1_clicked();
    void on_btnPage_Doctor_2_clicked();
    void on_btnPage_Doctor_3_clicked();

    //slot cho page_user
    void on_btnAddUser_clicked();
    void on_btnRemoveUser_clicked();
    void on_btnSearchUser_clicked();
    void on_btnViewUserDetail_clicked();
    void on_btnSortAZUser_clicked();
    void on_btnSortZAUser_clicked();

    void on_btnPrevPage_User_clicked();
    void on_btnNextPage_User_clicked();
    void on_btnPage_User_1_clicked();
    void on_btnPage_User_2_clicked();
    void on_btnPage_User_3_clicked();

    void on_logoutButton_clicked();

    void loadAppointmentData(int page = 1, const QString& searchText = "");
    void loadPatientData(int page = 1, const QString& searchText = "");
    void loadDoctorData(int page = 1, const QString& searchText = "");
    void loadUserData(int page = 1, const QString& searchText = "");
    // void loadMedicalRecordData(int page = 1, const QString& searchText = "");

    void loadAllManagersData();

private:
    Ui::AdminWindow *ui;
    //Appointment
    int currentAppointmentPage;
    int totalAppointmentPages;
    //Patient
    int currentPatientPage;
    int totalPatientPages;
    //Doctor
    int currentDoctorPage;
    int totalDoctorPages;
    //User
    int currentUserPage;
    int totalUserPages;

    int itemsPerPage;
    bool isPatientSortAscending = true;

    enum class PatientSortMode {
        BY_ID_ASC,
        BY_NAME_ASC,
        BY_NAME_DESC
    };

    enum class DoctorSortMode {
        BY_ID_ASC,
        BY_NAME_ASC,
        BY_NAME_DESC
    };

    enum class UserSortMode {
        BY_ID_ASC,
        BY_USERNAME_ASC,
        BY_USERNAME_DESC
    };

    DoctorSortMode currentDoctorSortMode = DoctorSortMode::BY_ID_ASC;
    PatientSortMode currentPatientSortMode = PatientSortMode::BY_ID_ASC;
    UserSortMode currentUserSortMode = UserSortMode::BY_ID_ASC;

    void setupPatientTable();
    void setupDoctorTable();
    void setupUserTable();

    void updateDoctorPaginationUI();
    void updatePatientPaginationUI();
    void updateUserPaginationUI();
};

#endif // ADMINWINDOW_H
