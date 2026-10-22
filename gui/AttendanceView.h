#ifndef ATTENDANCEVIEW_H
#define ATTENDANCEVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QSpinBox>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include "../AcademicManager.h"

class AttendanceView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    QComboBox* studentSelector;
    QSpinBox* attendedSpin;
    QSpinBox* totalSpin;
    QProgressBar* attendanceProgress;
    QLabel* statusBadge;
    QLabel* recoveryLabel;
    QDoubleSpinBox* thresholdSpin;

    // All Students Attendance Directory Table
    QTableWidget* attendanceTable;
    QComboBox* filterCombo;

    void setupUi();

public:
    explicit AttendanceView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();

signals:
    void dataModified();

private slots:
    void onStudentSelected(int index);
    void onSaveAttendance();
    void onThresholdChanged();
    void applyTableFilter();
};

#endif // ATTENDANCEVIEW_H
