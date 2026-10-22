#include "CurriculumView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QTextEdit>
#include <QFrame>
#include <QScrollArea>

CurriculumView::CurriculumView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setupUi();
    refreshData();
}

void CurriculumView::setupUi() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(16);

    QLabel* pageTitle = new QLabel("Subject Prerequisite Graph — Directed Acyclic Graph (DAG)", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Curriculum modeled as a DAG. Uses DFS traversal and Kahn's Topological Sort algorithm to determine registration eligibility.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    rootLayout->addWidget(pageTitle);
    rootLayout->addWidget(pageSubtitle);

    // Courses Table
    QFrame* coursesFrame = new QFrame(this);
    coursesFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 12px;");
    QVBoxLayout* coursesFLayout = new QVBoxLayout(coursesFrame);
    coursesFLayout->setSpacing(8);

    QLabel* coursesTitle = new QLabel("Registered Curriculum Courses (DAG Vertices & Edges)", coursesFrame);
    coursesTitle->setStyleSheet("font-size: 14px; font-weight: 700; color: #0F172A; border: none;");
    coursesFLayout->addWidget(coursesTitle);

    coursesTable = new QTableWidget(coursesFrame);
    coursesTable->setColumnCount(3);
    coursesTable->setHorizontalHeaderLabels({"Course Code", "Course Name", "Direct Prerequisites"});
    coursesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    coursesTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    coursesTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    coursesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    coursesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    coursesTable->verticalHeader()->setVisible(false);
    coursesTable->setMaximumHeight(240);
    coursesFLayout->addWidget(coursesTable);
    rootLayout->addWidget(coursesFrame);

    // Topological Order
    QFrame* topoFrame = new QFrame(this);
    topoFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 12px;");
    QVBoxLayout* topoLayout = new QVBoxLayout(topoFrame);

    QLabel* topoTitle = new QLabel("Optimal Course Completion Sequence (Kahn's Algorithm — Topological Sort)", topoFrame);
    topoTitle->setStyleSheet("font-size: 14px; font-weight: 700; color: #0F172A; border: none;");
    topoLayout->addWidget(topoTitle);

    topoOrderLabel = new QLabel("", topoFrame);
    topoOrderLabel->setWordWrap(true);
    topoOrderLabel->setStyleSheet("color: #1E40AF; font-weight: 600; padding: 8px; border: none; font-size: 13px;");
    topoLayout->addWidget(topoOrderLabel);
    rootLayout->addWidget(topoFrame);

    // Eligibility Checker
    QFrame* eligFrame = new QFrame(this);
    eligFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 16px;");
    QVBoxLayout* eligLayout = new QVBoxLayout(eligFrame);
    eligLayout->setSpacing(12);

    QLabel* eligTitle = new QLabel("Student Eligibility Checker (DFS Prerequisite Verification)", eligFrame);
    eligTitle->setStyleSheet("font-size: 14px; font-weight: 700; color: #0F172A; border: none;");
    eligLayout->addWidget(eligTitle);

    QHBoxLayout* eligControls = new QHBoxLayout();
    eligControls->setSpacing(12);

    QLabel* studLbl = new QLabel("Student:", eligFrame);
    studLbl->setStyleSheet("border: none; font-weight: 600;");
    studentSelector = new QComboBox(eligFrame);
    studentSelector->setMinimumWidth(250);

    QLabel* courseLbl = new QLabel("Target Course:", eligFrame);
    courseLbl->setStyleSheet("border: none; font-weight: 600;");
    targetCourseSelector = new QComboBox(eligFrame);
    targetCourseSelector->setMinimumWidth(200);

    QPushButton* checkBtn = new QPushButton("Check Eligibility (DFS)", eligFrame);
    checkBtn->setStyleSheet("background-color: #2563EB; color: white; padding: 8px 18px; border-radius: 6px; font-weight: 600;");
    connect(checkBtn, &QPushButton::clicked, this, &CurriculumView::checkStudentEligibility);

    eligControls->addWidget(studLbl);
    eligControls->addWidget(studentSelector);
    eligControls->addWidget(courseLbl);
    eligControls->addWidget(targetCourseSelector);
    eligControls->addWidget(checkBtn);
    eligControls->addStretch();
    eligLayout->addLayout(eligControls);

    eligibilityResultLabel = new QLabel("Select a student and a target course above, then click 'Check Eligibility'.", eligFrame);
    eligibilityResultLabel->setWordWrap(true);
    eligibilityResultLabel->setStyleSheet("padding: 10px 14px; background-color: #F8FAFC; border: 1px solid #E2E8F0; border-radius: 6px; color: #475569; font-size: 13px;");
    eligLayout->addWidget(eligibilityResultLabel);

    rootLayout->addWidget(eligFrame);
}

void CurriculumView::refreshData() {
    const CourseGraph& graph = manager->getCurriculumGraph();
    const auto& courses = graph.getCourses();

    coursesTable->setRowCount(0);
    for (const auto& pair : courses) {
        int r = coursesTable->rowCount();
        coursesTable->insertRow(r);
        coursesTable->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(pair.first)));
        coursesTable->setItem(r, 1, new QTableWidgetItem(QString::fromStdString(pair.second)));

        vector<string> prereqs = graph.getDirectPrerequisites(pair.first);
        QString prereqStr = prereqs.empty() ? "None (Foundation Course)" : "";
        for (size_t i = 0; i < prereqs.size(); ++i) {
            prereqStr += QString::fromStdString(prereqs[i]);
            if (i + 1 < prereqs.size()) prereqStr += " → ";
        }
        coursesTable->setItem(r, 2, new QTableWidgetItem(prereqStr));
    }

    // Topological Order (Kahn's Algo)
    vector<string> topoOrder = graph.getTopologicalOrder();
    QString topoStr = "📚 Recommended Learning Sequence:  ";
    for (size_t i = 0; i < topoOrder.size(); ++i) {
        topoStr += QString::fromStdString(topoOrder[i]);
        if (i + 1 < topoOrder.size()) topoStr += "  →  ";
    }
    topoOrderLabel->setText(topoStr);

    // Populate student selector
    studentSelector->clear();
    const auto& students = manager->getAllStudents();
    for (const auto& s : students) {
        studentSelector->addItem(
            QString("%1 — %2").arg(QString::fromStdString(s.getId()), QString::fromStdString(s.getName())),
            QString::fromStdString(s.getId())
        );
    }

    // Populate course selector
    targetCourseSelector->clear();
    for (const auto& pair : courses) {
        targetCourseSelector->addItem(
            QString("%1: %2").arg(QString::fromStdString(pair.first), QString::fromStdString(pair.second)),
            QString::fromStdString(pair.first)
        );
    }
}

void CurriculumView::checkStudentEligibility() {
    if (studentSelector->currentIndex() < 0 || targetCourseSelector->currentIndex() < 0) return;

    QString sId = studentSelector->currentData().toString();
    QString courseCode = targetCourseSelector->currentData().toString();

    const Student* s = manager->searchStudentById(sId.toStdString());
    if (!s) return;

    const CourseGraph& graph = manager->getCurriculumGraph();
    vector<string> missing;
    bool eligible = graph.checkEligibility(courseCode.toStdString(), s->getCompletedCourses(), missing);

    if (eligible) {
        eligibilityResultLabel->setText(
            QString("✅ <b>%1 (%2)</b> is <b style='color:#059669;'>ELIGIBLE</b> to register for <b>%3</b>. "
                    "All prerequisite courses have been successfully completed. Completed courses: %4")
            .arg(QString::fromStdString(s->getName()), sId, courseCode,
                 s->getCompletedCourses().empty() ? "None recorded" :
                 [&]() {
                     QString r;
                     for (const auto& c : s->getCompletedCourses()) r += QString::fromStdString(c) + " ";
                     return r;
                 }())
        );
        eligibilityResultLabel->setStyleSheet("padding: 10px 14px; background-color: #ECFDF5; border: 1px solid #A7F3D0; border-radius: 6px; color: #065F46; font-size: 13px;");
    } else {
        QString missingStr;
        for (const auto& m : missing) missingStr += QString::fromStdString(m) + ", ";
        if (!missingStr.isEmpty()) missingStr.chop(2);

        eligibilityResultLabel->setText(
            QString("❌ <b>%1 (%2)</b> is <b style='color:#DC2626;'>NOT ELIGIBLE</b> to register for <b>%3</b>. "
                    "Missing Prerequisites: <b>%4</b>. Complete these courses first.")
            .arg(QString::fromStdString(s->getName()), sId, courseCode, missingStr)
        );
        eligibilityResultLabel->setStyleSheet("padding: 10px 14px; background-color: #FEF2F2; border: 1px solid #FECACA; border-radius: 6px; color: #7F1D1D; font-size: 13px;");
    }
}
