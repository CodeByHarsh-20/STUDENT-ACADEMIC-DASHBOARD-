#include "DashboardView.h"
#include "../Analytics.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QGraphicsDropShadowEffect>

DashboardView::DashboardView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(20);

    // Header Title & Subtitle
    QVBoxLayout* headerLayout = new QVBoxLayout();
    headerLayout->setSpacing(4);
    QLabel* pageTitle = new QLabel("Academic Overview & Performance Metrics", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Real-time statistical synthesis derived directly from student academic records.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    headerLayout->addWidget(pageTitle);
    headerLayout->addWidget(pageSubtitle);
    rootLayout->addLayout(headerLayout);

    // 1. Five KPI Statistic Cards in a Grid
    QHBoxLayout* cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(16);

    cardsLayout->addWidget(createStatCard("Total Enrollment", totalStudentsVal, "Active student records", "#2563EB"));
    cardsLayout->addWidget(createStatCard("Class Average", avgMarksVal, "Weighted average mark", "#0284C7"));
    cardsLayout->addWidget(createStatCard("Mean CGPA", avgGpaVal, "10-point scale standard", "#0D9488"));
    cardsLayout->addWidget(createStatCard("Pass Rate", passRateVal, "Overall institutional rate", "#10B981"));
    cardsLayout->addWidget(createStatCard("Requires Attention", attentionVal, "At-risk or low attendance", "#EF4444"));

    rootLayout->addLayout(cardsLayout);

    // 2. Charts Section
    QHBoxLayout* chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(16);

    // Grade Distribution Bar Chart
    gradeChartView = new QChartView(this);
    gradeChartView->setRenderHint(QPainter::Antialiasing);
    gradeChartView->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px;");
    gradeChartView->setMinimumHeight(280);

    // Attendance Compliance Donut/Pie Chart
    attendanceChartView = new QChartView(this);
    attendanceChartView->setRenderHint(QPainter::Antialiasing);
    attendanceChartView->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px;");
    attendanceChartView->setMinimumHeight(280);

    chartsLayout->addWidget(gradeChartView, 3);
    chartsLayout->addWidget(attendanceChartView, 2);
    rootLayout->addLayout(chartsLayout);

    // 3. Quick Summary Tables
    QHBoxLayout* tablesLayout = new QHBoxLayout();
    tablesLayout->setSpacing(16);

    // Top Achievers Container
    QFrame* topFrame = new QFrame(this);
    topFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 12px;");
    QVBoxLayout* topFrameLayout = new QVBoxLayout(topFrame);
    topFrameLayout->setContentsMargins(8, 8, 8, 8);
    topFrameLayout->setSpacing(8);

    QLabel* topTitle = new QLabel("Top Academic Achievers (Dean's List Candidates)", topFrame);
    topTitle->setStyleSheet("font-size: 14px; font-weight: 700; color: #0F172A; border: none;");
    topFrameLayout->addWidget(topTitle);

    topStudentsTable = new QTableWidget(topFrame);
    topStudentsTable->setColumnCount(4);
    topStudentsTable->setHorizontalHeaderLabels({"ID", "Student Name", "Dept", "GPA"});
    topStudentsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    topStudentsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    topStudentsTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    topStudentsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    topStudentsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    topStudentsTable->verticalHeader()->setVisible(false);
    topStudentsTable->setMinimumHeight(180);
    topFrameLayout->addWidget(topStudentsTable);

    // Attention Needed Container
    QFrame* attnFrame = new QFrame(this);
    attnFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 12px;");
    QVBoxLayout* attnFrameLayout = new QVBoxLayout(attnFrame);
    attnFrameLayout->setContentsMargins(8, 8, 8, 8);
    attnFrameLayout->setSpacing(8);

    QLabel* attnTitle = new QLabel("Students Requiring Academic Attention", attnFrame);
    attnTitle->setStyleSheet("font-size: 14px; font-weight: 700; color: #DC2626; border: none;");
    attnFrameLayout->addWidget(attnTitle);

    attentionTable = new QTableWidget(attnFrame);
    attentionTable->setColumnCount(4);
    attentionTable->setHorizontalHeaderLabels({"ID", "Student Name", "Attendance", "Arrears"});
    attentionTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    attentionTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    attentionTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    attentionTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    attentionTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    attentionTable->verticalHeader()->setVisible(false);
    attentionTable->setMinimumHeight(180);
    attnFrameLayout->addWidget(attentionTable);

    tablesLayout->addWidget(topFrame);
    tablesLayout->addWidget(attnFrame);
    rootLayout->addLayout(tablesLayout);

    refreshData();
}

QWidget* DashboardView::createStatCard(const QString& title, QLabel*& valLabel, const QString& subtitle, const QString& accentColor) {
    QFrame* card = new QFrame(this);
    card->setStyleSheet(QString(
        "QFrame { background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; border-left: 4px solid %1; padding: 12px; }"
    ).arg(accentColor));

    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setContentsMargins(12, 10, 12, 10);
    cardLayout->setSpacing(4);

    QLabel* titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet("font-size: 12px; font-weight: 600; color: #64748B; border: none;");

    valLabel = new QLabel("0", card);
    valLabel->setStyleSheet(QString("font-size: 24px; font-weight: 800; color: %1; border: none;").arg(accentColor));

    QLabel* subLabel = new QLabel(subtitle, card);
    subLabel->setStyleSheet("font-size: 11px; color: #94A3B8; border: none;");

    cardLayout->addWidget(titleLabel);
    cardLayout->addWidget(valLabel);
    cardLayout->addWidget(subLabel);

    return card;
}

void DashboardView::refreshData() {
    const auto& students = manager->getAllStudents();

    if (students.empty()) {
        totalStudentsVal->setText("0");
        avgMarksVal->setText("0.0%");
        avgGpaVal->setText("0.00");
        passRateVal->setText("0.0%");
        attentionVal->setText("0");
        topStudentsTable->setRowCount(0);
        attentionTable->setRowCount(0);
        return;
    }

    ClassStatistics stats = Analytics::computeClassStatistics(students);
    totalStudentsVal->setText(QString::number(stats.totalStudents));
    avgMarksVal->setText(QString::number(stats.averagePercentage, 'f', 1) + "%");
    avgGpaVal->setText(QString::number(stats.averageGPA, 'f', 2));
    passRateVal->setText(QString::number(stats.passPercentage, 'f', 1) + "%");

    size_t attentionCount = stats.warningAttendanceCount + stats.debarredAttendanceCount;
    for (const auto& s : students) {
        if (s.hasFailingSubjects() && s.isAttendanceEligible()) {
            attentionCount++;
        }
    }
    attentionVal->setText(QString::number(attentionCount));

    // 1. Grade Distribution Chart
    auto gradeDist = Analytics::computeGradeDistribution(students);
    QBarSet* barSet = new QBarSet("Students");
    barSet->setColor(QColor("#2563EB"));
    QStringList gradeCategories = {"S", "A", "B", "C", "D", "E", "F"};
    for (const auto& cat : gradeCategories) {
        *barSet << gradeDist[cat.toStdString()];
    }

    QBarSeries* series = new QBarSeries();
    series->append(barSet);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Grade Distribution Breakdown");
    chart->setTitleFont(QFont("Segoe UI", 12, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(gradeCategories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    int maxGradeCount = 1;
    for (const auto& p : gradeDist) {
        if (p.second > maxGradeCount) maxGradeCount = p.second;
    }
    axisY->setRange(0, maxGradeCount + 2);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(false);

    QChart* oldGradeChart = gradeChartView->chart();
    gradeChartView->setChart(chart);
    delete oldGradeChart;

    // 2. Attendance Compliance Pie Chart
    QPieSeries* pieSeries = new QPieSeries();
    QPieSlice* sliceEligible = pieSeries->append(QString("Eligible (%1)").arg(stats.eligibleAttendanceCount), stats.eligibleAttendanceCount);
    QPieSlice* sliceWarning = pieSeries->append(QString("Warning (%1)").arg(stats.warningAttendanceCount), stats.warningAttendanceCount);
    QPieSlice* sliceDebarred = pieSeries->append(QString("Debarred (%1)").arg(stats.debarredAttendanceCount), stats.debarredAttendanceCount);

    sliceEligible->setColor(QColor("#10B981"));
    sliceWarning->setColor(QColor("#F59E0B"));
    sliceDebarred->setColor(QColor("#EF4444"));

    if (stats.debarredAttendanceCount > 0) {
        sliceDebarred->setExploded(true);
        sliceDebarred->setLabelVisible(true);
    }

    QChart* pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("Attendance Compliance Status");
    pieChart->setTitleFont(QFont("Segoe UI", 12, QFont::Bold));
    pieChart->setAnimationOptions(QChart::SeriesAnimations);
    pieChart->legend()->setAlignment(Qt::AlignRight);

    QChart* oldPieChart = attendanceChartView->chart();
    attendanceChartView->setChart(pieChart);
    delete oldPieChart;

    // 3. Top Students Table (from Heap Top-K)
    vector<Student> topK = Analytics::getTopKStudents(students, 5);
    topStudentsTable->setRowCount(static_cast<int>(topK.size()));
    for (int i = 0; i < static_cast<int>(topK.size()); ++i) {
        topStudentsTable->setItem(i, 0, new QTableWidgetItem(QString::fromStdString(topK[i].getId())));
        topStudentsTable->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(topK[i].getName())));
        topStudentsTable->setItem(i, 2, new QTableWidgetItem(QString::fromStdString(topK[i].getDepartment())));
        QTableWidgetItem* gpaItem = new QTableWidgetItem(QString::number(topK[i].calculateGPA(), 'f', 2));
        gpaItem->setTextAlignment(Qt::AlignCenter);
        gpaItem->setForeground(QBrush(QColor("#2563EB")));
        QFont font = gpaItem->font();
        font.setBold(true);
        gpaItem->setFont(font);
        topStudentsTable->setItem(i, 3, gpaItem);
    }

    // 4. Attention Needed Table
    attentionTable->setRowCount(0);
    int attnRow = 0;
    for (const auto& s : students) {
        bool lowAttn = s.getAttendancePercentage() < 75.0 && s.getTotalClasses() > 0;
        bool hasFails = s.hasFailingSubjects();
        if (lowAttn || hasFails) {
            attentionTable->insertRow(attnRow);
            attentionTable->setItem(attnRow, 0, new QTableWidgetItem(QString::fromStdString(s.getId())));
            attentionTable->setItem(attnRow, 1, new QTableWidgetItem(QString::fromStdString(s.getName())));

            QTableWidgetItem* attItem = new QTableWidgetItem(QString::number(s.getAttendancePercentage(), 'f', 1) + "%");
            attItem->setTextAlignment(Qt::AlignCenter);
            if (s.getAttendancePercentage() < 65.0) {
                attItem->setForeground(QBrush(QColor("#EF4444")));
            } else if (s.getAttendancePercentage() < 75.0) {
                attItem->setForeground(QBrush(QColor("#F59E0B")));
            } else {
                attItem->setForeground(QBrush(QColor("#10B981")));
            }
            attentionTable->setItem(attnRow, 2, attItem);

            QString arrearsStr = hasFails ? QString("%1 subject(s)").arg(s.getFailedSubjects().size()) : "None";
            QTableWidgetItem* arrItem = new QTableWidgetItem(arrearsStr);
            if (hasFails) {
                arrItem->setForeground(QBrush(QColor("#EF4444")));
            }
            attentionTable->setItem(attnRow, 3, arrItem);
            attnRow++;
        }
    }
}
