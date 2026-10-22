#include "MeritListView.h"
#include "../Analytics.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

MeritListView::MeritListView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setupUi();
    refreshData();
}

void MeritListView::setupUi() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(16);

    // Title
    QVBoxLayout* titleBox = new QVBoxLayout();
    titleBox->setSpacing(4);
    QLabel* pageTitle = new QLabel("Institutional Merit Ranking & Honors List", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Evaluate student cohorts using custom DSA sorting algorithms and Binary Search Trees.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    titleBox->addWidget(pageTitle);
    titleBox->addWidget(pageSubtitle);
    rootLayout->addLayout(titleBox);

    // Control Toolbar
    QFrame* controlFrame = new QFrame(this);
    controlFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 8px; padding: 10px;");
    QHBoxLayout* ctrlLayout = new QHBoxLayout(controlFrame);
    ctrlLayout->setContentsMargins(8, 4, 8, 4);
    ctrlLayout->setSpacing(14);

    QLabel* algoLbl = new QLabel("Ranking Algorithm (DSA):", controlFrame);
    algoLbl->setStyleSheet("font-weight: 700; color: #0F172A; border: none;");

    algorithmCombo = new QComboBox(controlFrame);
    algorithmCombo->addItem("1. Custom BST Reverse In-Order Traversal", 1);
    algorithmCombo->addItem("2. Custom QuickSort by GPA", 2);
    algorithmCombo->addItem("3. Custom Binary Max-Heap (Top 10 Achievers)", 3);
    algorithmCombo->setMinimumWidth(290);
    connect(algorithmCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MeritListView::generateMeritList);

    QLabel* deptLbl = new QLabel("Department:", controlFrame);
    deptLbl->setStyleSheet("border: none;");

    deptFilter = new QComboBox(controlFrame);
    deptFilter->addItems({"All Departments", "CSE", "ECE", "MECH", "IT", "EEE", "CIVIL"});
    connect(deptFilter, &QComboBox::currentTextChanged, this, &MeritListView::generateMeritList);

    QPushButton* refreshBtn = new QPushButton("Re-evaluate", controlFrame);
    refreshBtn->setStyleSheet("background-color: #2563EB; color: white; padding: 6px 16px; border-radius: 6px; font-weight: 600;");
    connect(refreshBtn, &QPushButton::clicked, this, &MeritListView::generateMeritList);

    ctrlLayout->addWidget(algoLbl);
    ctrlLayout->addWidget(algorithmCombo);
    ctrlLayout->addWidget(deptLbl);
    ctrlLayout->addWidget(deptFilter);
    ctrlLayout->addWidget(refreshBtn);
    ctrlLayout->addStretch();

    rootLayout->addWidget(controlFrame);

    // Complexity Information Note
    complexityLabel = new QLabel("", this);
    complexityLabel->setStyleSheet("background-color: #EFF6FF; border: 1px solid #BFDBFE; border-radius: 6px; padding: 8px 12px; color: #1E40AF; font-size: 12px; font-weight: 600;");
    rootLayout->addWidget(complexityLabel);

    // Merit Ranking Table
    meritTable = new QTableWidget(this);
    meritTable->setColumnCount(8);
    meritTable->setHorizontalHeaderLabels({"Rank", "Student ID", "Full Name", "Department", "Semester", "Overall %", "GPA", "Honor Standing"});
    meritTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    meritTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    meritTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    meritTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    meritTable->setAlternatingRowColors(true);
    meritTable->verticalHeader()->setVisible(false);

    rootLayout->addWidget(meritTable);
}

void MeritListView::refreshData() {
    generateMeritList();
}

void MeritListView::generateMeritList() {
    meritTable->setRowCount(0);
    int algoChoice = algorithmCombo->currentData().toInt();
    QString deptFilterText = deptFilter->currentText();

    vector<Student> ranked;

    if (algoChoice == 1) {
        // Custom BST Reverse In-Order
        ranked = manager->getMeritListBST();
        complexityLabel->setText("⚡ <b>Algorithm:</b> Custom Binary Search Tree (BST) Reverse In-Order Traversal &nbsp;|&nbsp; <b>Time Complexity:</b> O(N) linear in-order scan &nbsp;|&nbsp; <b>Space Complexity:</b> O(H) recursion stack height");
    } else if (algoChoice == 2) {
        // Custom QuickSort
        ranked = manager->getMeritListQuickSort();
        complexityLabel->setText("⚡ <b>Algorithm:</b> Custom QuickSort (Lomuto Partitioning by GPA) &nbsp;|&nbsp; <b>Time Complexity:</b> Average O(N log N) &nbsp;|&nbsp; <b>Space Complexity:</b> O(log N) stack frames");
    } else {
        // Heap Top-10
        ranked = Analytics::getTopKStudents(manager->getAllStudents(), 10);
        complexityLabel->setText("⚡ <b>Algorithm:</b> Custom Binary Max-Heap Priority Extraction &nbsp;|&nbsp; <b>Time Complexity:</b> O(N log K) bounded priority heap &nbsp;|&nbsp; <b>Space Complexity:</b> O(K) heap array");
    }

    int rank = 1;
    for (const auto& s : ranked) {
        if (deptFilterText != "All Departments" && QString::fromStdString(s.getDepartment()).compare(deptFilterText, Qt::CaseInsensitive) != 0) {
            continue;
        }

        int row = meritTable->rowCount();
        meritTable->insertRow(row);

        QString rankStr = QString("#%1").arg(rank);
        QTableWidgetItem* rankItem = new QTableWidgetItem(rankStr);
        rankItem->setTextAlignment(Qt::AlignCenter);
        QFont f = rankItem->font();
        f.setBold(true);
        rankItem->setFont(f);
        if (rank == 1) rankItem->setForeground(QBrush(QColor("#D97706"))); // Gold
        else if (rank == 2) rankItem->setForeground(QBrush(QColor("#64748B"))); // Silver
        else if (rank == 3) rankItem->setForeground(QBrush(QColor("#B45309"))); // Bronze
        meritTable->setItem(row, 0, rankItem);

        meritTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s.getId())));
        meritTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s.getName())));
        meritTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(s.getDepartment())));
        meritTable->setItem(row, 4, new QTableWidgetItem(QString("Sem %1").arg(s.getSemester())));

        QTableWidgetItem* pctItem = new QTableWidgetItem(QString::number(s.calculatePercentage(), 'f', 1) + "%");
        pctItem->setTextAlignment(Qt::AlignCenter);
        meritTable->setItem(row, 5, pctItem);

        QTableWidgetItem* gpaItem = new QTableWidgetItem(QString::number(s.calculateGPA(), 'f', 2));
        gpaItem->setTextAlignment(Qt::AlignCenter);
        gpaItem->setForeground(QBrush(QColor("#2563EB")));
        gpaItem->setFont(f);
        meritTable->setItem(row, 6, gpaItem);

        QString honor = (s.calculateGPA() >= 9.0) ? "⭐ Dean's List (Honors)" :
                        (s.calculateGPA() >= 8.0) ? "Distinction" :
                        (s.calculateGPA() >= 7.0) ? "First Class" : "Pass";
        QTableWidgetItem* honorItem = new QTableWidgetItem(honor);
        if (s.calculateGPA() >= 9.0) honorItem->setForeground(QBrush(QColor("#D97706")));
        else if (s.calculateGPA() >= 8.0) honorItem->setForeground(QBrush(QColor("#059669")));
        meritTable->setItem(row, 7, honorItem);

        rank++;
    }
}
