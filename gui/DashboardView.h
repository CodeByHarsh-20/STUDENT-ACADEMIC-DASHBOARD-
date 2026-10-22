#ifndef DASHBOARDVIEW_H
#define DASHBOARDVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include "../AcademicManager.h"

class DashboardView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    // Stat card value labels
    QLabel* totalStudentsVal;
    QLabel* avgMarksVal;
    QLabel* avgGpaVal;
    QLabel* passRateVal;
    QLabel* attentionVal;

    // Charts
    QChartView* gradeChartView;
    QChartView* attendanceChartView;

    // Quick Tables
    QTableWidget* topStudentsTable;
    QTableWidget* attentionTable;

    QWidget* createStatCard(const QString& title, QLabel*& valLabel, const QString& subtitle, const QString& accentColor);

public:
    explicit DashboardView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();
};

#endif // DASHBOARDVIEW_H
