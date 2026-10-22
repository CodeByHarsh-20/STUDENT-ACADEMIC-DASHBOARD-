#include "AnalyticsView.h"
#include "../Analytics.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

AnalyticsView::AnalyticsView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(18);

    // Title
    QVBoxLayout* titleBox = new QVBoxLayout();
    titleBox->setSpacing(4);
    QLabel* pageTitle = new QLabel("Institutional Performance Analytics", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Advanced academic statistics including standard deviation, normal distributions, and subject breakdown.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    titleBox->addWidget(pageTitle);
    titleBox->addWidget(pageSubtitle);
    rootLayout->addLayout(titleBox);

    // 6 Metrics Cards in a 2x3 Grid
    QGridLayout* gridLayout = new QGridLayout();
    gridLayout->setSpacing(14);

    gridLayout->addWidget(createMetricBox("Weighted Class Average", avgScoreVal, "Mean score across all subjects"), 0, 0);
    gridLayout->addWidget(createMetricBox("Mean Institutional GPA", avgGpaVal, "Grade Point Average (10.0 scale)"), 0, 1);
    gridLayout->addWidget(createMetricBox("Standard Deviation (σ)", stdDevVal, "Measures score dispersion"), 0, 2);
    gridLayout->addWidget(createMetricBox("Highest GPA Achieved", highestGpaVal, "Top academic performance"), 1, 0);
    gridLayout->addWidget(createMetricBox("Lowest GPA Recorded", lowestGpaVal, "Cohort lower boundary"), 1, 1);
    gridLayout->addWidget(createMetricBox("Institutional Pass Rate", passRateVal, "Scored >= 40.0% in all subjects"), 1, 2);

    rootLayout->addLayout(gridLayout);

    // Bottom Split: Left (Grade Histogram) | Right (Subject Breakdown Table)
    QHBoxLayout* splitLayout = new QHBoxLayout();
    splitLayout->setSpacing(16);

    histogramView = new QChartView(this);
    histogramView->setRenderHint(QPainter::Antialiasing);
    histogramView->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px;");
    histogramView->setMinimumHeight(280);
    splitLayout->addWidget(histogramView, 2);

    // Subject Breakdown Table Container
    QFrame* tblFrame = new QFrame(this);
    tblFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 12px;");
    QVBoxLayout* tblFrameLayout = new QVBoxLayout(tblFrame);
    tblFrameLayout->setSpacing(10);

    QLabel* tblTitle = new QLabel("Subject-Wise Evaluation Breakdown", tblFrame);
    tblTitle->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A; border: none;");
    tblFrameLayout->addWidget(tblTitle);

    subjectBreakdownTable = new QTableWidget(tblFrame);
    subjectBreakdownTable->setColumnCount(6);
    subjectBreakdownTable->setHorizontalHeaderLabels({"Subject Name", "Enrolled", "Average", "High", "Low", "Pass Rate"});
    subjectBreakdownTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    subjectBreakdownTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    subjectBreakdownTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    subjectBreakdownTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    subjectBreakdownTable->verticalHeader()->setVisible(false);
    tblFrameLayout->addWidget(subjectBreakdownTable);

    splitLayout->addWidget(tblFrame, 3);
    rootLayout->addLayout(splitLayout);

    refreshData();
}

QWidget* AnalyticsView::createMetricBox(const QString& title, QLabel*& valLabel, const QString& subtitle) {
    QFrame* box = new QFrame(this);
    box->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; padding: 12px;");

    QVBoxLayout* lay = new QVBoxLayout(box);
    lay->setContentsMargins(12, 10, 12, 10);
    lay->setSpacing(4);

    QLabel* titleLbl = new QLabel(title, box);
    titleLbl->setStyleSheet("font-size: 11px; font-weight: 700; color: #64748B; text-transform: uppercase; border: none;");

    valLabel = new QLabel("0.0", box);
    valLabel->setStyleSheet("font-size: 22px; font-weight: 800; color: #2563EB; border: none;");

    QLabel* subLbl = new QLabel(subtitle, box);
    subLbl->setStyleSheet("font-size: 11px; color: #94A3B8; border: none;");

    lay->addWidget(titleLbl);
    lay->addWidget(valLabel);
    lay->addWidget(subLbl);

    return box;
}

void AnalyticsView::refreshData() {
    const auto& students = manager->getAllStudents();
    if (students.empty()) {
        avgScoreVal->setText("0.0%");
        avgGpaVal->setText("0.00");
        stdDevVal->setText("0.00");
        highestGpaVal->setText("0.00");
        lowestGpaVal->setText("0.00");
        passRateVal->setText("0.0%");
        subjectBreakdownTable->setRowCount(0);
        return;
    }

    ClassStatistics stats = Analytics::computeClassStatistics(students);
    avgScoreVal->setText(QString::number(stats.averagePercentage, 'f', 1) + "%");
    avgGpaVal->setText(QString::number(stats.averageGPA, 'f', 2));
    stdDevVal->setText(QString::number(stats.standardDeviation, 'f', 2));
    highestGpaVal->setText(QString::number(stats.highestGPA, 'f', 2));
    lowestGpaVal->setText(QString::number(stats.lowestGPA, 'f', 2));
    passRateVal->setText(QString::number(stats.passPercentage, 'f', 1) + "%");

    // 1. Grade Distribution Histogram
    auto gradeDist = Analytics::computeGradeDistribution(students);
    QBarSet* set = new QBarSet("Students");
    set->setColor(QColor("#0284C7"));
    QStringList gradeLabels = {"S", "A", "B", "C", "D", "E", "F"};
    for (const auto& g : gradeLabels) {
        *set << gradeDist[g.toStdString()];
    }

    QBarSeries* series = new QBarSeries();
    series->append(set);

    QChart* chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Grade Frequency Distribution");
    chart->setTitleFont(QFont("Segoe UI", 12, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(gradeLabels);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis* axisY = new QValueAxis();
    int maxC = 1;
    for (const auto& p : gradeDist) if (p.second > maxC) maxC = p.second;
    axisY->setRange(0, maxC + 2);
    axisY->setLabelFormat("%d");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    chart->legend()->setVisible(false);

    QChart* old = histogramView->chart();
    histogramView->setChart(chart);
    delete old;

    // 2. Subject Breakdown
    struct SubStats {
        double sum = 0.0;
        double maxMark = -1.0;
        double minMark = 101.0;
        int count = 0;
        int passCount = 0;
    };
    std::map<string, SubStats> subMap;

    for (const auto& s : students) {
        for (const auto& pair : s.getMarks()) {
            SubStats& st = subMap[pair.first];
            st.sum += pair.second;
            st.count++;
            if (pair.second > st.maxMark) st.maxMark = pair.second;
            if (pair.second < st.minMark) st.minMark = pair.second;
            if (pair.second >= 40.0) st.passCount++;
        }
    }

    subjectBreakdownTable->setRowCount(0);
    for (const auto& pair : subMap) {
        int r = subjectBreakdownTable->rowCount();
        subjectBreakdownTable->insertRow(r);

        double avg = (pair.second.count > 0) ? (pair.second.sum / pair.second.count) : 0.0;
        double passPct = (pair.second.count > 0) ? ((double)pair.second.passCount / pair.second.count) * 100.0 : 0.0;

        subjectBreakdownTable->setItem(r, 0, new QTableWidgetItem(QString::fromStdString(pair.first)));
        subjectBreakdownTable->setItem(r, 1, new QTableWidgetItem(QString::number(pair.second.count)));
        subjectBreakdownTable->setItem(r, 2, new QTableWidgetItem(QString::number(avg, 'f', 1)));
        subjectBreakdownTable->setItem(r, 3, new QTableWidgetItem(QString::number(pair.second.maxMark, 'f', 1)));
        subjectBreakdownTable->setItem(r, 4, new QTableWidgetItem(QString::number(pair.second.minMark, 'f', 1)));

        QTableWidgetItem* passItem = new QTableWidgetItem(QString::number(passPct, 'f', 1) + "%");
        passItem->setForeground(QBrush(QColor(passPct >= 80.0 ? "#10B981" : (passPct >= 50.0 ? "#F59E0B" : "#EF4444"))));
        passItem->setTextAlignment(Qt::AlignCenter);
        subjectBreakdownTable->setItem(r, 5, passItem);
    }
}
