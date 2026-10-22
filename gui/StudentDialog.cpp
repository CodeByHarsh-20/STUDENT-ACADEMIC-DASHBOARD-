#include "StudentDialog.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

StudentDialog::StudentDialog(QWidget* parent, const Student* existingStudent)
    : QDialog(parent), isEditMode(existingStudent != nullptr) {
    setWindowTitle(isEditMode ? "Edit Student Record" : "Enroll New Student");
    setMinimumWidth(440);
    setStyleSheet("QDialog { background-color: #FFFFFF; font-size: 13px; }");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(16);
    mainLayout->setContentsMargins(24, 24, 24, 24);

    QLabel* titleLabel = new QLabel(isEditMode ? "Update Student Information" : "Add Student to Academic Directory", this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: 700; color: #0F172A; margin-bottom: 8px;");
    mainLayout->addWidget(titleLabel);

    QFormLayout* formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignLeft);

    idEdit = new QLineEdit(this);
    idEdit->setPlaceholderText("e.g. 24BCE1001");
    if (isEditMode && existingStudent) {
        idEdit->setText(QString::fromStdString(existingStudent->getId()));
        idEdit->setReadOnly(true);
        idEdit->setStyleSheet("background-color: #F1F5F9; color: #64748B;");
    }

    nameEdit = new QLineEdit(this);
    nameEdit->setPlaceholderText("Full Name");
    if (isEditMode && existingStudent) {
        nameEdit->setText(QString::fromStdString(existingStudent->getName()));
    }

    ageSpin = new QSpinBox(this);
    ageSpin->setRange(16, 99);
    ageSpin->setValue((isEditMode && existingStudent) ? existingStudent->getAge() : 19);

    emailEdit = new QLineEdit(this);
    emailEdit->setPlaceholderText("e.g. name@univ.edu");
    if (isEditMode && existingStudent) {
        emailEdit->setText(QString::fromStdString(existingStudent->getEmail()));
    }

    deptCombo = new QComboBox(this);
    deptCombo->setEditable(true);
    deptCombo->addItems({"CSE", "ECE", "MECH", "IT", "EEE", "CIVIL", "DATA SCIENCE"});
    if (isEditMode && existingStudent) {
        int idx = deptCombo->findText(QString::fromStdString(existingStudent->getDepartment()));
        if (idx >= 0) deptCombo->setCurrentIndex(idx);
        else deptCombo->setEditText(QString::fromStdString(existingStudent->getDepartment()));
    }

    semSpin = new QSpinBox(this);
    semSpin->setRange(1, 8);
    semSpin->setValue((isEditMode && existingStudent) ? existingStudent->getSemester() : 1);

    formLayout->addRow("Student ID *:", idEdit);
    formLayout->addRow("Full Name *:", nameEdit);
    formLayout->addRow("Age (16-99):", ageSpin);
    formLayout->addRow("Email Address *:", emailEdit);
    formLayout->addRow("Department *:", deptCombo);
    formLayout->addRow("Semester (1-8):", semSpin);

    mainLayout->addLayout(formLayout);

    errorLabel = new QLabel("", this);
    errorLabel->setStyleSheet("color: #EF4444; font-size: 12px; font-weight: 600;");
    errorLabel->setWordWrap(true);
    errorLabel->hide();
    mainLayout->addWidget(errorLabel);

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton* cancelBtn = new QPushButton("Cancel", this);
    cancelBtn->setProperty("class", "secondaryBtn");
    cancelBtn->setStyleSheet("background-color: #FFFFFF; color: #475569; border: 1px solid #CBD5E1; padding: 8px 16px; border-radius: 6px; font-weight: 600;");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    QPushButton* saveBtn = new QPushButton(isEditMode ? "Save Changes" : "Enroll Student", this);
    saveBtn->setStyleSheet("background-color: #2563EB; color: #FFFFFF; border: none; padding: 8px 18px; border-radius: 6px; font-weight: 600;");
    connect(saveBtn, &QPushButton::clicked, this, &StudentDialog::validateAndAccept);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);

    mainLayout->addLayout(btnLayout);
}

void StudentDialog::validateAndAccept() {
    QString id = idEdit->text().trimmed();
    QString name = nameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString dept = deptCombo->currentText().trimmed();

    if (id.isEmpty()) {
        errorLabel->setText("Student ID is mandatory.");
        errorLabel->show();
        idEdit->setFocus();
        return;
    }
    if (name.isEmpty()) {
        errorLabel->setText("Student full name cannot be blank.");
        errorLabel->show();
        nameEdit->setFocus();
        return;
    }
    if (email.isEmpty() || !email.contains("@") || !email.contains(".")) {
        errorLabel->setText("Please enter a valid email address.");
        errorLabel->show();
        emailEdit->setFocus();
        return;
    }
    if (dept.isEmpty()) {
        errorLabel->setText("Please select or specify a department.");
        errorLabel->show();
        deptCombo->setFocus();
        return;
    }

    errorLabel->hide();
    accept();
}

Student StudentDialog::getStudentData() const {
    Student s(idEdit->text().trimmed().toStdString(),
              nameEdit->text().trimmed().toStdString(),
              ageSpin->value(),
              emailEdit->text().trimmed().toStdString(),
              deptCombo->currentText().trimmed().toStdString(),
              semSpin->value());
    return s;
}

QString StudentDialog::getStudentId() const {
    return idEdit->text().trimmed();
}
