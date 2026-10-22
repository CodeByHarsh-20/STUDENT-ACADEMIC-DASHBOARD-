#include "MarksView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>

MarksView::MarksView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setupUi();
    refreshData();
}

void MarksView::setupUi() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(18);

    // Title Section
    QVBoxLayout* titleBox = new QVBoxLayout();
    titleBox->setSpacing(4);
    QLabel* pageTitle = new QLabel("Marks & Grades Management", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Record subject evaluations, compute GPA on 10.0 scale, and track university letter grades.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    titleBox->addWidget(pageTitle);
    titleBox->addWidget(pageSubtitle);
    rootLayout->addLayout(titleBox);

    // Student Selector Bar
    QFrame* selectorCard = new QFrame(this);
    selectorCard->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; padding: 12px;");
    QHBoxLayout* selLayout = new QHBoxLayout(selectorCard);
    selLayout->setContentsMargins(8, 4, 8, 4);
    selLayout->setSpacing(12);

    QLabel* selLabel = new QLabel("Select Student:", selectorCard);
    selLabel->setStyleSheet("font-weight: 700; color: #0F172A; border: none;");

    studentSelector = new QComboBox(selectorCard);
    studentSelector->setMinimumWidth(320);
    connect(studentSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MarksView::onStudentSelected);

    studentInfoLabel = new QLabel("", selectorCard);
    studentInfoLabel->setStyleSheet("color: #64748B; font-weight: 600; border: none;");

    selLayout->addWidget(selLabel);
    selLayout->addWidget(studentSelector);
    selLayout->addWidget(studentInfoLabel);
    selLayout->addStretch();
    rootLayout->addWidget(selectorCard);

    // Main Content Split: Left (Marks Table & Form) | Right (Academic Summary Badges)
    QHBoxLayout* contentLayout = new QHBoxLayout();
    contentLayout->setSpacing(16);

    // Left Column: Table and Marks Entry
    QVBoxLayout* leftCol = new QVBoxLayout();
    leftCol->setSpacing(12);

    // Marks Table
    marksTable = new QTableWidget(this);
    marksTable->setColumnCount(4);
    marksTable->setHorizontalHeaderLabels({"Subject Name", "Score / 100", "Grade Point", "Result"});
    marksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    marksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    marksTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    marksTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    marksTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    marksTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    marksTable->setSelectionMode(QAbstractItemView::SingleSelection);
    marksTable->verticalHeader()->setVisible(false);
    connect(marksTable, &QTableWidget::itemSelectionChanged, this, &MarksView::onMarkRowSelected);
    leftCol->addWidget(marksTable);

    // Mark Entry Form
    QFrame* formCard = new QFrame(this);
    formCard->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; padding: 12px;");
    QHBoxLayout* formLayout = new QHBoxLayout(formCard);
    formLayout->setContentsMargins(8, 6, 8, 6);
    formLayout->setSpacing(10);

    subjectEdit = new QLineEdit(formCard);
    subjectEdit->setPlaceholderText("Subject Name (e.g. Data Structures)");
    subjectEdit->setMinimumWidth(200);

    scoreSpin = new QDoubleSpinBox(formCard);
    scoreSpin->setRange(0.0, 100.0);
    scoreSpin->setDecimals(1);
    scoreSpin->setSingleStep(1.0);
    scoreSpin->setValue(85.0);
    scoreSpin->setSuffix(" / 100");

    saveMarkBtn = new QPushButton("Save Mark", formCard);
    saveMarkBtn->setStyleSheet("background-color: #2563EB; color: white; padding: 8px 16px; border-radius: 6px; font-weight: 600;");
    connect(saveMarkBtn, &QPushButton::clicked, this, &MarksView::onSaveMark);

    deleteMarkBtn = new QPushButton("Delete Mark", formCard);
    deleteMarkBtn->setStyleSheet("background-color: #FEE2E2; color: #DC2626; border: 1px solid #FCA5A5; padding: 8px 16px; border-radius: 6px; font-weight: 600;");
    deleteMarkBtn->setEnabled(false);
    connect(deleteMarkBtn, &QPushButton::clicked, this, &MarksView::onDeleteMark);

    formLayout->addWidget(new QLabel("Subject:", formCard));
    formLayout->addWidget(subjectEdit, 2);
    formLayout->addWidget(new QLabel("Score:", formCard));
    formLayout->addWidget(scoreSpin, 1);
    formLayout->addWidget(saveMarkBtn);
    formLayout->addWidget(deleteMarkBtn);

    leftCol->addWidget(formCard);
    contentLayout->addLayout(leftCol, 3);

    // Right Column: Academic Report Summary Card
    QFrame* summaryCard = new QFrame(this);
    summaryCard->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 16px;");
    QVBoxLayout* sumLayout = new QVBoxLayout(summaryCard);
    sumLayout->setContentsMargins(16, 16, 16, 16);
    sumLayout->setSpacing(14);

    QLabel* sumTitle = new QLabel("Current Academic Standing", summaryCard);
    sumTitle->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A; border: none;");
    sumLayout->addWidget(sumTitle);

    // GPA KPI
    QFrame* gpaBox = new QFrame(summaryCard);
    gpaBox->setStyleSheet("background-color: #EFF6FF; border: 1px solid #BFDBFE; border-radius: 8px; padding: 12px;");
    QVBoxLayout* gpaBoxLayout = new QVBoxLayout(gpaBox);
    QLabel* gpaTitle = new QLabel("CUMULATIVE GPA (10.0 Scale)", gpaBox);
    gpaTitle->setStyleSheet("font-size: 11px; font-weight: 700; color: #1E40AF; border: none;");
    gpaBadge = new QLabel("0.00", gpaBox);
    gpaBadge->setStyleSheet("font-size: 32px; font-weight: 800; color: #2563EB; border: none;");
    gpaBoxLayout->addWidget(gpaTitle);
    gpaBoxLayout->addWidget(gpaBadge);
    sumLayout->addWidget(gpaBox);

    // Score & Grade Boxes
    QGridLayout* metricsGrid = new QGridLayout();
    metricsGrid->setSpacing(10);

    QLabel* pctTitle = new QLabel("Overall Average:", summaryCard);
    pctTitle->setStyleSheet("color: #64748B; font-weight: 600; border: none;");
    percentageBadge = new QLabel("0.0%", summaryCard);
    percentageBadge->setStyleSheet("font-size: 18px; font-weight: 700; color: #0F172A; border: none;");

    QLabel* grdTitle = new QLabel("Letter Grade:", summaryCard);
    grdTitle->setStyleSheet("color: #64748B; font-weight: 600; border: none;");
    gradeBadge = new QLabel("N/A", summaryCard);
    gradeBadge->setStyleSheet("font-size: 18px; font-weight: 700; color: #0F172A; border: none;");

    metricsGrid->addWidget(pctTitle, 0, 0);
    metricsGrid->addWidget(percentageBadge, 0, 1);
    metricsGrid->addWidget(grdTitle, 1, 0);
    metricsGrid->addWidget(gradeBadge, 1, 1);
    sumLayout->addLayout(metricsGrid);

    // Arrear Status
    statusBadge = new QLabel("No Failing Subjects", summaryCard);
    statusBadge->setStyleSheet("padding: 8px 12px; background-color: #ECFDF5; color: #059669; border-radius: 6px; font-weight: 600; border: 1px solid #A7F3D0;");
    statusBadge->setWordWrap(true);
    sumLayout->addWidget(statusBadge);

    sumLayout->addStretch();
    contentLayout->addWidget(summaryCard, 2);

    rootLayout->addLayout(contentLayout);
}

void MarksView::refreshData() {
    studentSelector->blockSignals(true);
    QString currentSelectedId = "";
    if (studentSelector->currentIndex() >= 0) {
        currentSelectedId = studentSelector->currentData().toString();
    }

    studentSelector->clear();
    const auto& students = manager->getAllStudents();

    int restoreIdx = 0;
    for (size_t i = 0; i < students.size(); ++i) {
        QString sId = QString::fromStdString(students[i].getId());
        QString label = QString("%1 — %2 (%3, Sem %4)")
            .arg(sId, QString::fromStdString(students[i].getName()),
                 QString::fromStdString(students[i].getDepartment()),
                 QString::number(students[i].getSemester()));
        studentSelector->addItem(label, sId);
        if (sId == currentSelectedId) {
            restoreIdx = static_cast<int>(i);
        }
    }

    studentSelector->blockSignals(false);
    if (studentSelector->count() > 0) {
        studentSelector->setCurrentIndex(restoreIdx);
        onStudentSelected(restoreIdx);
    } else {
        marksTable->setRowCount(0);
        studentInfoLabel->setText("");
        gpaBadge->setText("0.00");
        percentageBadge->setText("0.0%");
        gradeBadge->setText("N/A");
        statusBadge->setText("No students enrolled.");
    }
}

void MarksView::onStudentSelected(int index) {
    if (index < 0 || studentSelector->count() == 0) return;
    QString sId = studentSelector->currentData().toString();
    const Student* s = manager->searchStudentById(sId.toStdString());
    if (!s) return;

    studentInfoLabel->setText(QString("Department: %1 | Semester: %2")
        .arg(QString::fromStdString(s->getDepartment()), QString::number(s->getSemester())));

    // Populate Marks Table
    marksTable->setRowCount(0);
    deleteMarkBtn->setEnabled(false);

    for (const auto& pair : s->getMarks()) {
        int r = marksTable->rowCount();
        marksTable->insertRow(r);

        marksTable->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(pair.first)));
        marksTable->setItem(r, 1, new QTableWidgetItem(QString::number(pair.second, 'f', 1)));

        // GPA Points for mark
        double pts = 0.0;
        if (pair.second >= 90.0) pts = 10.0;
        else if (pair.second >= 80.0) pts = 9.0;
        else if (pair.second >= 70.0) pts = 8.0;
        else if (pair.second >= 60.0) pts = 7.0;
        else if (pair.second >= 50.0) pts = 6.0;
        else if (pair.second >= 40.0) pts = 5.0;

        marksTable->setItem(r, 2, new QTableWidgetItem(QString::number(pts, 'f', 1)));

        bool pass = pair.second >= 40.0;
        QTableWidgetItem* resItem = new QTableWidgetItem(pass ? "PASS" : "FAIL (Arrear)");
        resItem->setForeground(QBrush(QColor(pass ? "#10B981" : "#EF4444")));
        QFont f = resItem->font();
        f.setBold(true);
        resItem->setFont(f);
        marksTable->setItem(r, 3, resItem);
    }

    // Refresh Summary Badges
    gpaBadge->setText(QString::number(s->calculateGPA(), 'f', 2));
    percentageBadge->setText(QString::number(s->calculatePercentage(), 'f', 1) + "%");
    gradeBadge->setText(QString::fromStdString(s->calculateGrade()));

    if (s->hasFailingSubjects()) {
        auto fails = s->getFailedSubjects();
        QString failStr = "⚠️ Arrears: ";
        for (const auto& f : fails) failStr += QString::fromStdString(f) + " ";
        statusBadge->setText(failStr);
        statusBadge->setStyleSheet("padding: 8px 12px; background-color: #FEF2F2; color: #DC2626; border-radius: 6px; font-weight: 600; border: 1px solid #FECACA;");
    } else {
        statusBadge->setText("✓ All registered subjects cleared");
        statusBadge->setStyleSheet("padding: 8px 12px; background-color: #ECFDF5; color: #059669; border-radius: 6px; font-weight: 600; border: 1px solid #A7F3D0;");
    }
}

void MarksView::onMarkRowSelected() {
    int curRow = marksTable->currentRow();
    if (curRow >= 0) {
        subjectEdit->setText(marksTable->item(curRow, 0)->text());
        scoreSpin->setValue(marksTable->item(curRow, 1)->text().toDouble());
        deleteMarkBtn->setEnabled(true);
    } else {
        deleteMarkBtn->setEnabled(false);
    }
}

void MarksView::onSaveMark() {
    if (studentSelector->currentIndex() < 0) return;
    QString sId = studentSelector->currentData().toString();
    QString subject = subjectEdit->text().trimmed();
    double score = scoreSpin->value();

    if (subject.isEmpty()) {
        QMessageBox::warning(this, "Missing Input", "Please specify a subject name.");
        return;
    }

    try {
        if (manager->setStudentMark(sId.toStdString(), subject.toStdString(), score)) {
            onStudentSelected(studentSelector->currentIndex());
            emit dataModified();
            subjectEdit->clear();
        }
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Mark Save Error", QString::fromStdString(ex.what()));
    }
}

void MarksView::onDeleteMark() {
    int curRow = marksTable->currentRow();
    if (curRow < 0) return;

    QString sId = studentSelector->currentData().toString();
    Student* s = const_cast<Student*>(manager->searchStudentById(sId.toStdString()));
    if (!s) return;

    QString subject = marksTable->item(curRow, 0)->text();
    if (s->removeMark(subject.toStdString())) {
        onStudentSelected(studentSelector->currentIndex());
        emit dataModified();
        subjectEdit->clear();
        deleteMarkBtn->setEnabled(false);
    }
}
