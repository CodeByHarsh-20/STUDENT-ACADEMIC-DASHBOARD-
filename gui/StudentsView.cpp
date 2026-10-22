#include "StudentsView.h"
#include "StudentDialog.h"
#include "../Exceptions.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QDialog>
#include <QTextEdit>

StudentsView::StudentsView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setupUi();
    refreshTable();
}

void StudentsView::setupUi() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(16);

    // Title & Actions Bar
    QHBoxLayout* topHeader = new QHBoxLayout();
    QVBoxLayout* titleBox = new QVBoxLayout();
    titleBox->setSpacing(4);
    QLabel* pageTitle = new QLabel("Student Directory & Academic Enrollment", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Manage enrollment profiles, departmental assignments, and performance summaries.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    titleBox->addWidget(pageTitle);
    titleBox->addWidget(pageSubtitle);
    topHeader->addLayout(titleBox);

    QPushButton* addBtn = new QPushButton("+ Enroll Student", this);
    addBtn->setStyleSheet("background-color: #2563EB; color: white; padding: 10px 20px; font-weight: 600; border-radius: 6px;");
    connect(addBtn, &QPushButton::clicked, this, &StudentsView::onAddStudent);
    topHeader->addWidget(addBtn);

    rootLayout->addLayout(topHeader);

    // Filter Toolbar
    QFrame* filterFrame = new QFrame(this);
    filterFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; padding: 8px;");
    QHBoxLayout* filterLayout = new QHBoxLayout(filterFrame);
    filterLayout->setContentsMargins(8, 4, 8, 4);
    filterLayout->setSpacing(12);

    QLabel* searchIcon = new QLabel("🔍", filterFrame);
    searchEdit = new QLineEdit(filterFrame);
    searchEdit->setPlaceholderText("Search by Student ID or Name...");
    searchEdit->setClearButtonEnabled(true);
    searchEdit->setMinimumWidth(260);
    connect(searchEdit, &QLineEdit::textChanged, this, &StudentsView::applyFilter);

    deptFilter = new QComboBox(filterFrame);
    deptFilter->addItems({"All Departments", "CSE", "ECE", "MECH", "IT", "EEE", "CIVIL", "DATA SCIENCE"});
    connect(deptFilter, &QComboBox::currentTextChanged, this, &StudentsView::applyFilter);

    semFilter = new QComboBox(filterFrame);
    semFilter->addItems({"All Semesters", "Sem 1", "Sem 2", "Sem 3", "Sem 4", "Sem 5", "Sem 6", "Sem 7", "Sem 8"});
    connect(semFilter, &QComboBox::currentTextChanged, this, &StudentsView::applyFilter);

    countLabel = new QLabel("0 records", filterFrame);
    countLabel->setStyleSheet("color: #64748B; font-weight: 600;");

    filterLayout->addWidget(searchIcon);
    filterLayout->addWidget(searchEdit);
    filterLayout->addWidget(new QLabel("Department:", filterFrame));
    filterLayout->addWidget(deptFilter);
    filterLayout->addWidget(new QLabel("Semester:", filterFrame));
    filterLayout->addWidget(semFilter);
    filterLayout->addStretch();
    filterLayout->addWidget(countLabel);

    rootLayout->addWidget(filterFrame);

    // Students Table
    studentTable = new QTableWidget(this);
    studentTable->setColumnCount(9);
    studentTable->setHorizontalHeaderLabels({
        "Student ID", "Full Name", "Department", "Sem", "Attendance %", "Score %", "GPA", "Grade", "Status"
    });
    studentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    studentTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    studentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    studentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    studentTable->setSelectionMode(QAbstractItemView::SingleSelection);
    studentTable->setAlternatingRowColors(true);
    studentTable->verticalHeader()->setVisible(false);
    studentTable->setSortingEnabled(true);
    connect(studentTable, &QTableWidget::doubleClicked, this, &StudentsView::onViewDetails);

    rootLayout->addWidget(studentTable);

    // Table Actions Bar
    QHBoxLayout* actionsLayout = new QHBoxLayout();
    QPushButton* viewBtn = new QPushButton("View Academic Transcript", this);
    viewBtn->setStyleSheet("background-color: #FFFFFF; color: #334155; border: 1px solid #CBD5E1; padding: 8px 16px; border-radius: 6px; font-weight: 600;");
    connect(viewBtn, &QPushButton::clicked, this, &StudentsView::onViewDetails);

    QPushButton* editBtn = new QPushButton("Edit Student Record", this);
    editBtn->setStyleSheet("background-color: #FFFFFF; color: #334155; border: 1px solid #CBD5E1; padding: 8px 16px; border-radius: 6px; font-weight: 600;");
    connect(editBtn, &QPushButton::clicked, this, &StudentsView::onEditStudent);

    QPushButton* deleteBtn = new QPushButton("Delete Student", this);
    deleteBtn->setStyleSheet("background-color: #FEE2E2; color: #DC2626; border: 1px solid #FCA5A5; padding: 8px 16px; border-radius: 6px; font-weight: 600;");
    connect(deleteBtn, &QPushButton::clicked, this, &StudentsView::onDeleteStudent);

    actionsLayout->addWidget(viewBtn);
    actionsLayout->addWidget(editBtn);
    actionsLayout->addWidget(deleteBtn);
    actionsLayout->addStretch();

    rootLayout->addLayout(actionsLayout);
}

void StudentsView::refreshTable() {
    applyFilter();
}

void StudentsView::applyFilter() {
    studentTable->setSortingEnabled(false);
    studentTable->setRowCount(0);

    const auto& students = manager->getAllStudents();
    QString query = searchEdit->text().trimmed().toLower();
    QString dept = deptFilter->currentText();
    QString semText = semFilter->currentText();
    int semNum = 0;
    if (semText.startsWith("Sem ")) {
        semNum = semText.mid(4).toInt();
    }

    int visibleCount = 0;
    for (const auto& s : students) {
        QString sId = QString::fromStdString(s.getId());
        QString sName = QString::fromStdString(s.getName());
        QString sDept = QString::fromStdString(s.getDepartment());

        // Search text check
        if (!query.isEmpty()) {
            if (!sId.toLower().contains(query) && !sName.toLower().contains(query)) {
                continue;
            }
        }

        // Dept check
        if (dept != "All Departments" && sDept.compare(dept, Qt::CaseInsensitive) != 0) {
            continue;
        }

        // Sem check
        if (semNum > 0 && s.getSemester() != semNum) {
            continue;
        }

        int row = studentTable->rowCount();
        studentTable->insertRow(row);

        studentTable->setItem(row, 0, new QTableWidgetItem(sId));
        studentTable->setItem(row, 1, new QTableWidgetItem(sName));
        studentTable->setItem(row, 2, new QTableWidgetItem(sDept));

        QTableWidgetItem* semItem = new QTableWidgetItem();
        semItem->setData(Qt::DisplayRole, s.getSemester());
        studentTable->setItem(row, 3, semItem);

        QTableWidgetItem* attItem = new QTableWidgetItem();
        attItem->setData(Qt::DisplayRole, s.getAttendancePercentage());
        attItem->setText(QString::number(s.getAttendancePercentage(), 'f', 1) + "%");
        attItem->setTextAlignment(Qt::AlignCenter);
        if (s.getAttendancePercentage() < 65.0) {
            attItem->setForeground(QBrush(QColor("#EF4444")));
        } else if (s.getAttendancePercentage() < 75.0) {
            attItem->setForeground(QBrush(QColor("#F59E0B")));
        } else {
            attItem->setForeground(QBrush(QColor("#10B981")));
        }
        studentTable->setItem(row, 4, attItem);

        QTableWidgetItem* scoreItem = new QTableWidgetItem();
        scoreItem->setData(Qt::DisplayRole, s.calculatePercentage());
        scoreItem->setText(QString::number(s.calculatePercentage(), 'f', 1) + "%");
        scoreItem->setTextAlignment(Qt::AlignCenter);
        studentTable->setItem(row, 5, scoreItem);

        QTableWidgetItem* gpaItem = new QTableWidgetItem();
        gpaItem->setData(Qt::DisplayRole, s.calculateGPA());
        gpaItem->setText(QString::number(s.calculateGPA(), 'f', 2));
        gpaItem->setTextAlignment(Qt::AlignCenter);
        gpaItem->setForeground(QBrush(QColor("#2563EB")));
        QFont font = gpaItem->font();
        font.setBold(true);
        gpaItem->setFont(font);
        studentTable->setItem(row, 6, gpaItem);

        QTableWidgetItem* gradeItem = new QTableWidgetItem(QString::fromStdString(s.calculateGrade()));
        studentTable->setItem(row, 7, gradeItem);

        QString statusStr = s.isAttendanceEligible() ? "Eligible" : (s.getAttendancePercentage() >= 65.0 ? "Warning" : "Debarred");
        QTableWidgetItem* statusItem = new QTableWidgetItem(statusStr);
        statusItem->setTextAlignment(Qt::AlignCenter);
        if (statusStr == "Eligible") statusItem->setForeground(QBrush(QColor("#10B981")));
        else if (statusStr == "Warning") statusItem->setForeground(QBrush(QColor("#F59E0B")));
        else statusItem->setForeground(QBrush(QColor("#EF4444")));
        studentTable->setItem(row, 8, statusItem);

        visibleCount++;
    }

    studentTable->setSortingEnabled(true);
    countLabel->setText(QString("%1 students listed").arg(visibleCount));
}

void StudentsView::onAddStudent() {
    StudentDialog dlg(this, nullptr);
    if (dlg.exec() == QDialog::Accepted) {
        Student newStudent = dlg.getStudentData();
        try {
            if (manager->addStudent(newStudent, true)) {
                refreshTable();
                emit dataModified();
                QMessageBox::information(this, "Student Enrolled", 
                    QString("Student %1 (%2) has been enrolled successfully into the academic directory.")
                    .arg(QString::fromStdString(newStudent.getId()), QString::fromStdString(newStudent.getName())));
            }
        } catch (const exception& ex) {
            QMessageBox::warning(this, "Enrollment Error", QString::fromStdString(ex.what()));
        }
    }
}

void StudentsView::onEditStudent() {
    int curRow = studentTable->currentRow();
    if (curRow < 0) {
        QMessageBox::information(this, "Select Student", "Please select a student from the directory table to edit.");
        return;
    }

    QString id = studentTable->item(curRow, 0)->text();
    const Student* existing = manager->searchStudentById(id.toStdString());
    if (!existing) {
        QMessageBox::warning(this, "Not Found", "Selected student record could not be found.");
        return;
    }

    StudentDialog dlg(this, existing);
    if (dlg.exec() == QDialog::Accepted) {
        Student updated = dlg.getStudentData();
        try {
            manager->updateStudentRecord(id.toStdString(), updated.getName(), updated.getAge(),
                                         updated.getEmail(), updated.getDepartment(), updated.getSemester());
            refreshTable();
            emit dataModified();
            QMessageBox::information(this, "Record Updated", "Student information was successfully updated.");
        } catch (const exception& ex) {
            QMessageBox::warning(this, "Update Error", QString::fromStdString(ex.what()));
        }
    }
}

void StudentsView::onDeleteStudent() {
    int curRow = studentTable->currentRow();
    if (curRow < 0) {
        QMessageBox::information(this, "Select Student", "Please select a student record to delete.");
        return;
    }

    QString id = studentTable->item(curRow, 0)->text();
    QString name = studentTable->item(curRow, 1)->text();

    auto reply = QMessageBox::question(this, "Confirm Deletion",
        QString("Are you sure you want to permanently delete student %1 (%2)?\n\n(This action can be undone via the Undo toolbar button).")
        .arg(id, name), QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        try {
            if (manager->deleteStudent(id.toStdString(), true)) {
                refreshTable();
                emit dataModified();
                QMessageBox::information(this, "Deleted", QString("Student record %1 deleted.").arg(id));
            }
        } catch (const exception& ex) {
            QMessageBox::warning(this, "Delete Error", QString::fromStdString(ex.what()));
        }
    }
}

void StudentsView::onViewDetails() {
    int curRow = studentTable->currentRow();
    if (curRow < 0) return;

    QString id = studentTable->item(curRow, 0)->text();
    const Student* s = manager->searchStudentById(id.toStdString());
    if (!s) return;

    QDialog* reportDlg = new QDialog(this);
    reportDlg->setWindowTitle(QString("Academic Transcript — %1 (%2)").arg(QString::fromStdString(s->getName()), id));
    reportDlg->setMinimumSize(600, 520);
    reportDlg->setStyleSheet("background-color: #FFFFFF; font-size: 13px;");

    QVBoxLayout* layout = new QVBoxLayout(reportDlg);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(14);

    QLabel* title = new QLabel(QString("Official Academic Transcript: %1").arg(QString::fromStdString(s->getName())), reportDlg);
    title->setStyleSheet("font-size: 18px; font-weight: 700; color: #0F172A;");
    layout->addWidget(title);

    QTextEdit* details = new QTextEdit(reportDlg);
    details->setReadOnly(true);
    details->setStyleSheet("border: 1px solid #CBD5E1; border-radius: 6px; padding: 12px; font-family: 'Consolas', monospace; font-size: 13px; line-height: 1.5;");

    QString reportHtml = QString(R"(
        <h3 style='color:#1E3A8A; margin-bottom:4px;'>STUDENT PROFILE</h3>
        <b>ID:</b> %1 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Name:</b> %2 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Age:</b> %3<br>
        <b>Email:</b> %4<br>
        <b>Department:</b> %5 &nbsp;&nbsp;|&nbsp;&nbsp; <b>Semester:</b> %6<br>
        <hr color='#E2E8F0'>
        <h3 style='color:#1E3A8A; margin-bottom:4px;'>ATTENDANCE COMPLIANCE</h3>
        <b>Classes Attended:</b> %7 / %8 (%9%) &nbsp;&nbsp; <b>Status:</b> %10<br>
        <hr color='#E2E8F0'>
        <h3 style='color:#1E3A8A; margin-bottom:4px;'>SUBJECT-WISE ACADEMIC PERFORMANCE</h3>
    )")
    .arg(QString::fromStdString(s->getId()),
         QString::fromStdString(s->getName()),
         QString::number(s->getAge()),
         QString::fromStdString(s->getEmail()),
         QString::fromStdString(s->getDepartment()),
         QString::number(s->getSemester()),
         QString::number(s->getAttendedClasses()),
         QString::number(s->getTotalClasses()),
         QString::number(s->getAttendancePercentage(), 'f', 1),
         QString::fromStdString(s->getAttendanceStatus()));

    if (s->getMarks().empty()) {
        reportHtml += "<p style='color:#64748B;'><i>[No subject marks recorded yet]</i></p>";
    } else {
        reportHtml += "<table border='1' cellspacing='0' cellpadding='6' style='border-collapse:collapse; width:100%; border-color:#CBD5E1;'>";
        reportHtml += "<tr style='background-color:#F8FAFC;'><th>Subject Name</th><th>Score / 100</th><th>Status</th></tr>";
        for (const auto& pair : s->getMarks()) {
            bool pass = pair.second >= 40.0;
            reportHtml += QString("<tr><td>%1</td><td align='center'><b>%2</b></td><td align='center' style='color:%3;'><b>%4</b></td></tr>")
                .arg(QString::fromStdString(pair.first),
                     QString::number(pair.second, 'f', 1),
                     pass ? "#10B981" : "#EF4444",
                     pass ? "PASS" : "FAIL (Arrear)");
        }
        reportHtml += "</table>";
    }

    reportHtml += QString(R"(
        <hr color='#E2E8F0'>
        <table style='width:100%; font-size:14px;'>
        <tr>
            <td><b>Overall Percentage:</b> <span style='color:#2563EB;'>%1%</span></td>
            <td><b>GPA (10.0 scale):</b> <span style='color:#2563EB; font-weight:bold;'>%2</span></td>
            <td><b>Letter Grade:</b> <span style='font-weight:bold;'>%3</span></td>
        </tr>
        </table>
    )")
    .arg(QString::number(s->calculatePercentage(), 'f', 2),
         QString::number(s->calculateGPA(), 'f', 2),
         QString::fromStdString(s->calculateGrade()));

    details->setHtml(reportHtml);
    layout->addWidget(details);

    QPushButton* closeBtn = new QPushButton("Close", reportDlg);
    closeBtn->setStyleSheet("background-color: #2563EB; color: white; padding: 8px 18px; border-radius: 6px; font-weight: 600;");
    connect(closeBtn, &QPushButton::clicked, reportDlg, &QDialog::accept);
    layout->addWidget(closeBtn, 0, Qt::AlignRight);

    reportDlg->exec();
}
