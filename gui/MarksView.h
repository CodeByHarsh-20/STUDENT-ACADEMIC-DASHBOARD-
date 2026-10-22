#ifndef MARKSVIEW_H
#define MARKSVIEW_H

#include <QWidget>
#include <QComboBox>
#include <QTableWidget>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include "../AcademicManager.h"

class MarksView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    QComboBox* studentSelector;
    QLabel* studentInfoLabel;
    QTableWidget* marksTable;

    // Form inputs for mark entry
    QLineEdit* subjectEdit;
    QDoubleSpinBox* scoreSpin;
    QPushButton* saveMarkBtn;
    QPushButton* deleteMarkBtn;

    // Academic summary badges
    QLabel* gpaBadge;
    QLabel* percentageBadge;
    QLabel* gradeBadge;
    QLabel* statusBadge;

    void setupUi();

public:
    explicit MarksView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();

signals:
    void dataModified();

private slots:
    void onStudentSelected(int index);
    void onSaveMark();
    void onDeleteMark();
    void onMarkRowSelected();
};

#endif // MARKSVIEW_H
