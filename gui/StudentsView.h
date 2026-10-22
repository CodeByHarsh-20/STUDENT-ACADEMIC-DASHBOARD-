#ifndef STUDENTSVIEW_H
#define STUDENTSVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "../AcademicManager.h"

class StudentsView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    QLineEdit* searchEdit;
    QComboBox* deptFilter;
    QComboBox* semFilter;
    QTableWidget* studentTable;
    QLabel* countLabel;

    void setupUi();
    void applyFilter();

public:
    explicit StudentsView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshTable();

signals:
    void dataModified();

private slots:
    void onAddStudent();
    void onEditStudent();
    void onDeleteStudent();
    void onViewDetails();
};

#endif // STUDENTSVIEW_H
