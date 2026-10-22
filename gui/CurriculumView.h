#ifndef CURRICULUMVIEW_H
#define CURRICULUMVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include "../AcademicManager.h"

class CurriculumView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    QTableWidget* coursesTable;
    QComboBox* studentSelector;
    QComboBox* targetCourseSelector;
    QLabel* eligibilityResultLabel;
    QLabel* topoOrderLabel;

    void setupUi();

public:
    explicit CurriculumView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();

private slots:
    void checkStudentEligibility();
};

#endif // CURRICULUMVIEW_H
