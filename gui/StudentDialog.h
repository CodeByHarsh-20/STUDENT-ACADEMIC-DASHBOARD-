#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include "../Student.h"

class StudentDialog : public QDialog {
    Q_OBJECT
private:
    QLineEdit* idEdit;
    QLineEdit* nameEdit;
    QSpinBox* ageSpin;
    QLineEdit* emailEdit;
    QComboBox* deptCombo;
    QSpinBox* semSpin;
    QLabel* errorLabel;
    bool isEditMode;

public:
    explicit StudentDialog(QWidget* parent = nullptr, const Student* existingStudent = nullptr);

    Student getStudentData() const;
    QString getStudentId() const;

private slots:
    void validateAndAccept();
};

#endif // STUDENTDIALOG_H
