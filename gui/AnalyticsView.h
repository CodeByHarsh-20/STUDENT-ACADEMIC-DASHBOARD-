#ifndef ANALYTICSVIEW_H
#define ANALYTICSVIEW_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QtCharts/QChartView>
#include "../AcademicManager.h"

class AnalyticsView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    // Metrics labels
    QLabel* avgScoreVal;
    QLabel* avgGpaVal;
    QLabel* highestGpaVal;
    QLabel* lowestGpaVal;
    QLabel* stdDevVal;
    QLabel* passRateVal;

    QChartView* histogramView;
    QTableWidget* subjectBreakdownTable;

    QWidget* createMetricBox(const QString& title, QLabel*& valLabel, const QString& subtitle);

public:
    explicit AnalyticsView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();
};

#endif // ANALYTICSVIEW_H
