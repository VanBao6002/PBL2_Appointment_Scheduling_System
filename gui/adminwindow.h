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

    //slot cho page_medicalrecord
    void on_btnAddMedicalRecord_clicked();
    void on_btnEditMedicalRecord_clicked();
    void on_btnRemoveMedicalRecord_clicked();
    void on_btnSearchMedicalRecord_clicked();
    void on_btnViewMedicalRecordDetail_clicked();
    void on_btnSortAZMedicalRecord_clicked();
    void on_btnSortZAMedicalRecord_clicked();

    void on_btnPrevPage_MedicalRecord_clicked();
    void on_btnNextPage_MedicalRecord_clicked();
    void on_btnPage_MedicalRecord_1_clicked();
    void on_btnPage_MedicalRecord_2_clicked();
    void on_btnPage_MedicalRecord_3_clicked();

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
    void loadMedicalRecordData(int page = 1, const QString& searchText = "");
    void loadUserData(int page = 1, const QString& searchText = "");
   

public:
    void setUserID(int id) { userID = id; }
    int getUserID() const { return userID; }
private:
    Ui::AdminWindow *ui;
    int userID = 0;
    bool hasAppointmentAccess(bool write = false) const;
    bool hasPatientAccess(bool write = false) const;
    bool hasDoctorAccess(bool write = false) const;
    bool hasUserAccess(bool write = false) const;
    bool hasMedicalRecordAccess(bool write = false) const;
    void showNoPermissionMessage() const;
    //Appointment
    int currentAppointmentPage;
    int totalAppointmentPages;
    //Patient
    int currentPatientPage;
    int totalPatientPages;
    //Doctor
    int currentDoctorPage;
    int totalDoctorPages;
    //Medical Record
    int currentMedicalRecordPage;
    int totalMedicalRecordPages;
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

    enum class MedicalRecordSortMode {
        BY_ID_ASC,
        BY_PATIENT_NAME_ASC,
        BY_PATIENT_NAME_DESC
    };

    enum class UserSortMode {
        BY_ID_ASC,
        BY_USERNAME_ASC,
        BY_USERNAME_DESC
    };

    DoctorSortMode currentDoctorSortMode = DoctorSortMode::BY_ID_ASC;
    PatientSortMode currentPatientSortMode = PatientSortMode::BY_ID_ASC;
    MedicalRecordSortMode currentMedicalRecordSortMode = MedicalRecordSortMode::BY_ID_ASC;
    UserSortMode currentUserSortMode = UserSortMode::BY_ID_ASC;

    void setupPatientTable();
    void setupDoctorTable();
    void setupUserTable();
    void setupMedicalRecordTable();

    void updateAppointmentPaginationUI();
    void updateDoctorPaginationUI();
    void updatePatientPaginationUI();
    void updateUserPaginationUI();
    void updateMedicalRecordPaginationUI();

    void applyModernStyles();
    void setActiveSidebarButton(QPushButton* activeButton);

    protected:
};

#endif // ADMINWINDOW_H
