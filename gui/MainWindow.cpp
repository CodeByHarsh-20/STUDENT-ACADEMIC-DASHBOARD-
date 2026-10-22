#include "MainWindow.h"
#include "StyleTheme.h"
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCloseEvent>
#include <QLabel>
#include <QFont>
#include <QScrollArea>
#include <QMessageBox>
#include <QApplication>
#include <QTimer>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    manager = new AcademicManager();

    setWindowTitle("Student Academic Dashboard  |  University Academic Management System");
    setMinimumSize(1280, 780);
    resize(1400, 860);

    setStyleSheet(StyleTheme::getGlobalStyleSheet());

    // Central widget: sidebar (left) + content (right)
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setObjectName("centralHost");
    setCentralWidget(centralWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // --- LEFT SIDEBAR ---
    QWidget* sidebar = new QWidget(this);
    sidebar->setFixedWidth(230);
    sidebar->setStyleSheet("background-color: #0F172A;");
    mainLayout->addWidget(sidebar);
    setupSidebar(sidebar);

    // --- RIGHT CONTENT AREA ---
    QWidget* rightPanel = new QWidget(this);
    rightPanel->setStyleSheet("background-color: #F8FAFC;");
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(0);

    // Top Bar
    QWidget* topBar = new QWidget(rightPanel);
    topBar->setFixedHeight(58);
    topBar->setStyleSheet("background-color: #FFFFFF; border-bottom: 1px solid #E2E8F0;");
    setupTopBar(topBar);
    rightLayout->addWidget(topBar);

    // Scrollable content area
    QScrollArea* scrollArea = new QScrollArea(rightPanel);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet("QScrollArea { background-color: #F8FAFC; border: none; }");

    // Stacked Content Widget
    contentStack = new QStackedWidget(scrollArea);
    contentStack->setStyleSheet("background-color: #F8FAFC;");

    // Create Views
    dashboardView   = new DashboardView(manager, contentStack);
    studentsView    = new StudentsView(manager, contentStack);
    marksView       = new MarksView(manager, contentStack);
    attendanceView  = new AttendanceView(manager, contentStack);
    analyticsView   = new AnalyticsView(manager, contentStack);
    meritListView   = new MeritListView(manager, contentStack);
    curriculumView  = new CurriculumView(manager, contentStack);
    dataManView     = new DataManagementView(manager, contentStack);

    contentStack->addWidget(dashboardView);
    contentStack->addWidget(studentsView);
    contentStack->addWidget(marksView);
    contentStack->addWidget(attendanceView);
    contentStack->addWidget(analyticsView);
    contentStack->addWidget(meritListView);
    contentStack->addWidget(curriculumView);
    contentStack->addWidget(dataManView);

    scrollArea->setWidget(contentStack);
    rightLayout->addWidget(scrollArea);

    mainLayout->addWidget(rightPanel);

    // Connect dataModified signals
    connect(studentsView,  &StudentsView::dataModified,       this, &MainWindow::onDataModified);
    connect(marksView,     &MarksView::dataModified,          this, &MainWindow::onDataModified);
    connect(attendanceView,&AttendanceView::dataModified,     this, &MainWindow::onDataModified);
    connect(dataManView,   &DataManagementView::dataModified, this, &MainWindow::onDataModified);

    switchView(0);
    updateUndoRedoLabel();
}

void MainWindow::setupSidebar(QWidget* sidebar) {
    QVBoxLayout* sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(0, 0, 0, 0);
    sideLayout->setSpacing(0);

    // App Branding
    QWidget* brandWidget = new QWidget(sidebar);
    brandWidget->setFixedHeight(64);
    brandWidget->setStyleSheet("background-color: #1E293B;");
    QVBoxLayout* brandLayout = new QVBoxLayout(brandWidget);
    brandLayout->setContentsMargins(16, 8, 16, 8);
    brandLayout->setSpacing(2);

    QLabel* appName = new QLabel("Academic Manager", brandWidget);
    appName->setStyleSheet("color: #FFFFFF; font-size: 15px; font-weight: 800; border: none;");
    QLabel* appVer = new QLabel("University Dashboard v2.0", brandWidget);
    appVer->setStyleSheet("color: #64748B; font-size: 11px; border: none;");

    brandLayout->addWidget(appName);
    brandLayout->addWidget(appVer);
    sideLayout->addWidget(brandWidget);

    // Divider
    QWidget* div1 = new QWidget(sidebar);
    div1->setFixedHeight(1);
    div1->setStyleSheet("background-color: #1E293B;");
    sideLayout->addWidget(div1);

    // Nav section label
    QLabel* navSectionLabel = new QLabel("  NAVIGATION", sidebar);
    navSectionLabel->setStyleSheet("color: #475569; font-size: 10px; font-weight: 700; letter-spacing: 1px; padding: 14px 16px 4px 16px; border: none;");
    sideLayout->addWidget(navSectionLabel);

    struct NavItem { QString icon; QString label; };
    QList<NavItem> navItems = {
        {"🏠", "Dashboard"},
        {"👥", "Students"},
        {"📊", "Marks & Grades"},
        {"📅", "Attendance"},
        {"📈", "Analytics"},
        {"🏆", "Merit List"},
        {"🗂️", "Curriculum (DAG)"},
        {"💾", "Data Management"}
    };

    for (int i = 0; i < navItems.size(); ++i) {
        QPushButton* btn = createNavButton(navItems[i].icon, navItems[i].label, i);
        navButtons.append(btn);
        sideLayout->addWidget(btn);
    }

    sideLayout->addStretch();

    // Bottom footer
    QWidget* footer = new QWidget(sidebar);
    footer->setStyleSheet("background-color: #1E293B;");
    QVBoxLayout* footerLayout = new QVBoxLayout(footer);
    footerLayout->setContentsMargins(16, 8, 16, 8);

    QLabel* footerNote = new QLabel("Data Structures & Algorithms\nC++ Academic Project", footer);
    footerNote->setStyleSheet("color: #475569; font-size: 10px; border: none;");
    footerNote->setAlignment(Qt::AlignCenter);
    footerLayout->addWidget(footerNote);
    sideLayout->addWidget(footer);
}

QPushButton* MainWindow::createNavButton(const QString& icon, const QString& label, int index) {
    QPushButton* btn = new QPushButton(QString("  %1   %2").arg(icon, label), this);
    btn->setFixedHeight(44);
    btn->setStyleSheet(R"(
        QPushButton {
            background-color: transparent;
            color: #94A3B8;
            border: none;
            text-align: left;
            padding-left: 20px;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton:hover {
            background-color: #1E293B;
            color: #E2E8F0;
        }
    )");
    connect(btn, &QPushButton::clicked, this, [this, index]() { switchView(index); });
    return btn;
}

void MainWindow::setupTopBar(QWidget* topBar) {
    QHBoxLayout* topLayout = new QHBoxLayout(topBar);
    topLayout->setContentsMargins(20, 0, 20, 0);
    topLayout->setSpacing(12);

    QLabel* pageLabel = new QLabel("Academic Dashboard", topBar);
    pageLabel->setObjectName("topBarPageLabel");
    pageLabel->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A;");
    topLayout->addWidget(pageLabel);
    topLayout->addStretch();

    // Undo / Redo buttons
    QPushButton* undoBtn = new QPushButton("↩ Undo", topBar);
    undoBtn->setStyleSheet("background-color: #F1F5F9; color: #334155; border: 1px solid #CBD5E1; padding: 6px 12px; border-radius: 5px; font-weight: 600; font-size: 12px;");
    connect(undoBtn, &QPushButton::clicked, this, &MainWindow::onUndo);

    QPushButton* redoBtn = new QPushButton("Redo ↪", topBar);
    redoBtn->setStyleSheet("background-color: #F1F5F9; color: #334155; border: 1px solid #CBD5E1; padding: 6px 12px; border-radius: 5px; font-weight: 600; font-size: 12px;");
    connect(redoBtn, &QPushButton::clicked, this, &MainWindow::onRedo);

    undoRedoLabel = new QLabel("", topBar);
    undoRedoLabel->setStyleSheet("color: #64748B; font-size: 11px;");

    // Quick Save
    QPushButton* saveBtn = new QPushButton("💾 Quick Save", topBar);
    saveBtn->setStyleSheet("background-color: #2563EB; color: white; border: none; padding: 6px 14px; border-radius: 5px; font-weight: 600; font-size: 12px;");
    connect(saveBtn, &QPushButton::clicked, this, &MainWindow::onQuickSave);

    topLayout->addWidget(undoBtn);
    topLayout->addWidget(redoBtn);
    topLayout->addWidget(undoRedoLabel);
    topLayout->addWidget(saveBtn);
}

void MainWindow::switchView(int index) {
    contentStack->setCurrentIndex(index);

    // Update nav button styles
    for (int i = 0; i < navButtons.size(); ++i) {
        if (i == index) {
            navButtons[i]->setStyleSheet(R"(
                QPushButton {
                    background-color: #2563EB;
                    color: #FFFFFF;
                    border: none;
                    border-left: 3px solid #93C5FD;
                    text-align: left;
                    padding-left: 17px;
                    font-size: 13px;
                    font-weight: 700;
                }
            )");
        } else {
            navButtons[i]->setStyleSheet(R"(
                QPushButton {
                    background-color: transparent;
                    color: #94A3B8;
                    border: none;
                    text-align: left;
                    padding-left: 20px;
                    font-size: 13px;
                    font-weight: 500;
                }
                QPushButton:hover {
                    background-color: #1E293B;
                    color: #E2E8F0;
                }
            )");
        }
    }

    // Refresh the active view
    switch (index) {
        case 0: dashboardView->refreshData(); break;
        case 1: studentsView->refreshTable(); break;
        case 2: marksView->refreshData(); break;
        case 3: attendanceView->refreshData(); break;
        case 4: analyticsView->refreshData(); break;
        case 5: meritListView->refreshData(); break;
        case 6: curriculumView->refreshData(); break;
        case 7: dataManView->refreshData(); break;
    }
}

void MainWindow::onDataModified() {
    updateUndoRedoLabel();
    // Refresh currently active view to reflect changes
    switchView(contentStack->currentIndex());
}

void MainWindow::onUndo() {
    if (manager->undo()) {
        onDataModified();
    } else {
        QMessageBox::information(this, "Undo", "Nothing left to undo.");
    }
}

void MainWindow::onRedo() {
    if (manager->redo()) {
        onDataModified();
    } else {
        QMessageBox::information(this, "Redo", "Nothing left to redo.");
    }
}

void MainWindow::onQuickSave() {
    if (manager->saveRecords()) {
        // Brief visual feedback — no intrusive popup
        QLabel* feedback = new QLabel(" ✓ Saved ", centralWidget());
        feedback->setStyleSheet("background-color: #10B981; color: white; padding: 4px 10px; border-radius: 4px; font-weight: 700; font-size: 12px;");
        feedback->setGeometry(width() - 130, 4, 100, 30);
        feedback->show();
        QTimer::singleShot(2000, feedback, &QLabel::deleteLater);
    }
}

void MainWindow::updateUndoRedoLabel() {
    size_t undoC = manager->getUndoCount();
    size_t redoC = manager->getRedoCount();
    if (undoC == 0 && redoC == 0) {
        undoRedoLabel->setText("");
    } else {
        undoRedoLabel->setText(QString("(%1 undo, %2 redo)").arg(undoC).arg(redoC));
    }
}

void MainWindow::closeEvent(QCloseEvent* event) {
    manager->saveRecords();
    delete manager;
    event->accept();
}
