#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QTimer>
#include "../AcademicManager.h"
#include "DashboardView.h"
#include "StudentsView.h"
#include "MarksView.h"
#include "AttendanceView.h"
#include "AnalyticsView.h"
#include "MeritListView.h"
#include "CurriculumView.h"
#include "DataManagementView.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
private:
    AcademicManager* manager;

    // Views
    QStackedWidget* contentStack;
    DashboardView* dashboardView;
    StudentsView* studentsView;
    MarksView* marksView;
    AttendanceView* attendanceView;
    AnalyticsView* analyticsView;
    MeritListView* meritListView;
    CurriculumView* curriculumView;
    DataManagementView* dataManView;

    // Sidebar buttons
    QList<QPushButton*> navButtons;

    // Toolbar
    QLabel* undoRedoLabel;

    void setupSidebar(QWidget* sidebar);
    void setupTopBar(QWidget* topBar);
    QPushButton* createNavButton(const QString& icon, const QString& label, int index);

private slots:
    void switchView(int index);
    void onDataModified();
    void onUndo();
    void onRedo();
    void onQuickSave();
    void updateUndoRedoLabel();

protected:
    void closeEvent(QCloseEvent* event) override;
};

#endif // MAINWINDOW_H
