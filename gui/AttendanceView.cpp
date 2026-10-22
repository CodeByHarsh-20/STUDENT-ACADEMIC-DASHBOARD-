#include "AttendanceView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <cmath>

AttendanceView::AttendanceView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setupUi();
    refreshData();
}

void AttendanceView::setupUi() {
    QVBoxLayout* rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(24, 24, 24, 24);
    rootLayout->setSpacing(18);

    // Title
    QVBoxLayout* titleBox = new QVBoxLayout();
    titleBox->setSpacing(4);
    QLabel* pageTitle = new QLabel("Attendance Compliance & Eligibility Management", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Track class attendance, verify examination eligibility, and calculate attendance recovery projections.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    titleBox->addWidget(pageTitle);
    titleBox->addWidget(pageSubtitle);
    rootLayout->addLayout(titleBox);

    // Split Layout: Top (Editor Card + Recovery Calculator) | Bottom (Full Attendance Table)
    QHBoxLayout* topCardsLayout = new QHBoxLayout();
    topCardsLayout->setSpacing(16);

    // Left Card: Attendance Record Editor
    QFrame* editorCard = new QFrame(this);
    editorCard->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 16px;");
    QVBoxLayout* editLayout = new QVBoxLayout(editorCard);
    editLayout->setSpacing(14);

    QLabel* editTitle = new QLabel("Record Attendance Log", editorCard);
    editTitle->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A; border: none;");
    editLayout->addWidget(editTitle);

    QHBoxLayout* selRow = new QHBoxLayout();
    QLabel* selLbl = new QLabel("Student:", editorCard);
    selLbl->setStyleSheet("font-weight: 600; border: none;");
    studentSelector = new QComboBox(editorCard);
    studentSelector->setMinimumWidth(280);
    connect(studentSelector, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &AttendanceView::onStudentSelected);
    selRow->addWidget(selLbl);
    selRow->addWidget(studentSelector);
    selRow->addStretch();
    editLayout->addLayout(selRow);

    QGridLayout* spinGrid = new QGridLayout();
    spinGrid->setSpacing(12);

    QLabel* attLbl = new QLabel("Attended Classes:", editorCard);
    attLbl->setStyleSheet("border: none;");
    attendedSpin = new QSpinBox(editorCard);
    attendedSpin->setRange(0, 1000);

    QLabel* totLbl = new QLabel("Total Conducted Classes:", editorCard);
    totLbl->setStyleSheet("border: none;");
    totalSpin = new QSpinBox(editorCard);
    totalSpin->setRange(0, 1000);

    spinGrid->addWidget(attLbl, 0, 0);
    spinGrid->addWidget(attendedSpin, 0, 1);
    spinGrid->addWidget(totLbl, 1, 0);
    spinGrid->addWidget(totalSpin, 1, 1);
    editLayout->addLayout(spinGrid);

    QPushButton* saveAttBtn = new QPushButton("Update Attendance Record", editorCard);
    saveAttBtn->setStyleSheet("background-color: #2563EB; color: white; padding: 8px 18px; border-radius: 6px; font-weight: 600;");
    connect(saveAttBtn, &QPushButton::clicked, this, &AttendanceView::onSaveAttendance);
    editLayout->addWidget(saveAttBtn, 0, Qt::AlignRight);

    topCardsLayout->addWidget(editorCard, 3);

    // Right Card: Eligibility & Recovery Calculator
    QFrame* statusCard = new QFrame(this);
    statusCard->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 16px;");
    QVBoxLayout* statLayout = new QVBoxLayout(statusCard);
    statLayout->setSpacing(12);

    QLabel* statTitle = new QLabel("Attendance Compliance & Recovery", statusCard);
    statTitle->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A; border: none;");
    statLayout->addWidget(statTitle);

    QHBoxLayout* threshRow = new QHBoxLayout();
    QLabel* threshLbl = new QLabel("Mandatory Eligibility Threshold (%):", statusCard);
    threshLbl->setStyleSheet("border: none; color: #64748B;");
    thresholdSpin = new QDoubleSpinBox(statusCard);
    thresholdSpin->setRange(50.0, 100.0);
    thresholdSpin->setValue(75.0);
    thresholdSpin->setSuffix("%");
    connect(thresholdSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &AttendanceView::onThresholdChanged);
    threshRow->addWidget(threshLbl);
    threshRow->addWidget(thresholdSpin);
    statLayout->addLayout(threshRow);

    attendanceProgress = new QProgressBar(statusCard);
    attendanceProgress->setRange(0, 100);
    attendanceProgress->setValue(0);
    attendanceProgress->setTextVisible(true);
    attendanceProgress->setStyleSheet(R"(
        QProgressBar {
            background-color: #F1F5F9;
            border: 1px solid #CBD5E1;
            border-radius: 6px;
            height: 22px;
            text-align: center;
            font-weight: 700;
        }
        QProgressBar::chunk {
            background-color: #2563EB;
            border-radius: 5px;
        }
    )");
    statLayout->addWidget(attendanceProgress);

    statusBadge = new QLabel("STATUS: N/A", statusCard);
    statusBadge->setStyleSheet("padding: 8px 12px; border-radius: 6px; font-weight: 700;");
    statLayout->addWidget(statusBadge);

    recoveryLabel = new QLabel("", statusCard);
    recoveryLabel->setWordWrap(true);
    recoveryLabel->setStyleSheet("color: #475569; font-size: 12px; border: none; line-height: 1.4;");
    statLayout->addWidget(recoveryLabel);

    statLayout->addStretch();
    topCardsLayout->addWidget(statusCard, 2);

    rootLayout->addLayout(topCardsLayout);

    // Bottom Section: All Students Attendance Table
    QFrame* tableFrame = new QFrame(this);
    tableFrame->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 12px;");
    QVBoxLayout* tableFrameLayout = new QVBoxLayout(tableFrame);
    tableFrameLayout->setSpacing(10);

    QHBoxLayout* tableHeaderRow = new QHBoxLayout();
    QLabel* tblTitle = new QLabel("Cohort Attendance Roster", tableFrame);
    tblTitle->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A; border: none;");
    tableHeaderRow->addWidget(tblTitle);
    tableHeaderRow->addStretch();

    tableHeaderRow->addWidget(new QLabel("Filter by Compliance:", tableFrame));
    filterCombo = new QComboBox(tableFrame);
    filterCombo->addItems({"All Students", "Eligible (>= 75%)", "Warning (65% - 74%)", "Debarred (< 65%)"});
    connect(filterCombo, &QComboBox::currentTextChanged, this, &AttendanceView::applyTableFilter);
    tableHeaderRow->addWidget(filterCombo);

    tableFrameLayout->addLayout(tableHeaderRow);

    attendanceTable = new QTableWidget(tableFrame);
    attendanceTable->setColumnCount(7);
    attendanceTable->setHorizontalHeaderLabels({"Student ID", "Full Name", "Dept", "Attended", "Total", "Percentage", "Compliance"});
    attendanceTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    attendanceTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    attendanceTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    attendanceTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    attendanceTable->setSelectionMode(QAbstractItemView::SingleSelection);
    attendanceTable->verticalHeader()->setVisible(false);
    attendanceTable->setSortingEnabled(true);
    tableFrameLayout->addWidget(attendanceTable);

    rootLayout->addWidget(tableFrame);
}

void AttendanceView::refreshData() {
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
        QString label = QString("%1 — %2 (%3)")
            .arg(sId, QString::fromStdString(students[i].getName()),
                 QString::fromStdString(students[i].getDepartment()));
        studentSelector->addItem(label, sId);
        if (sId == currentSelectedId) {
            restoreIdx = static_cast<int>(i);
        }
    }

    studentSelector->blockSignals(false);
    if (studentSelector->count() > 0) {
        studentSelector->setCurrentIndex(restoreIdx);
        onStudentSelected(restoreIdx);
    }

    applyTableFilter();
}

void AttendanceView::onStudentSelected(int index) {
    if (index < 0 || studentSelector->count() == 0) return;
    QString sId = studentSelector->currentData().toString();
    const Student* s = manager->searchStudentById(sId.toStdString());
    if (!s) return;

    attendedSpin->setValue(s->getAttendedClasses());
    totalSpin->setValue(s->getTotalClasses());

    onThresholdChanged();
}

void AttendanceView::onThresholdChanged() {
    if (studentSelector->currentIndex() < 0) return;
    QString sId = studentSelector->currentData().toString();
    const Student* s = manager->searchStudentById(sId.toStdString());
    if (!s) return;

    int attended = attendedSpin->value();
    int total = totalSpin->value();
    double pct = (total == 0) ? 100.0 : ((double)attended / total) * 100.0;
    attendanceProgress->setValue(static_cast<int>(pct));

    double thresh = thresholdSpin->value();

    if (total == 0) {
        statusBadge->setText("STATUS: NO SESSIONS LOGGED");
        statusBadge->setStyleSheet("background-color: #F1F5F9; color: #475569; padding: 8px 12px; border-radius: 6px; font-weight: 700;");
        recoveryLabel->setText("No classes have been recorded yet for this student.");
    } else if (pct >= thresh) {
        statusBadge->setText(QString("STATUS: ELIGIBLE FOR EXAMINATIONS (%1%)").arg(QString::number(pct, 'f', 1)));
        statusBadge->setStyleSheet("background-color: #ECFDF5; color: #059669; border: 1px solid #A7F3D0; padding: 8px 12px; border-radius: 6px; font-weight: 700;");

        // Calculate buffer classes that can be missed
        // (attended) / (total + miss) >= thresh / 100 => attended >= 0.75 * total + 0.75 * miss => miss = (attended - 0.75*total)/0.75
        int buffer = static_cast<int>((attended - (thresh / 100.0) * total) / (thresh / 100.0));
        if (buffer > 0) {
            recoveryLabel->setText(QString("✓ Student comfortably exceeds the %1% threshold. Can safely miss up to <b>%2</b> upcoming class(es) while retaining exam eligibility.").arg(QString::number(thresh), QString::number(buffer)));
        } else {
            recoveryLabel->setText(QString("✓ Student meets the %1% threshold. Any missed class will trigger an attendance warning.").arg(QString::number(thresh)));
        }
    } else if (pct >= 65.0) {
        statusBadge->setText(QString("STATUS: ATTENDANCE WARNING (%1%)").arg(QString::number(pct, 'f', 1)));
        statusBadge->setStyleSheet("background-color: #FFFBEB; color: #D97706; border: 1px solid #FDE68A; padding: 8px 12px; border-radius: 6px; font-weight: 700;");

        // Classes needed to reach thresh:
        // (A + x) / (T + x) = P => x = (P*T - A)/(1 - P)
        double P = thresh / 100.0;
        int needed = static_cast<int>(std::ceil((P * total - attended) / (1.0 - P)));
        recoveryLabel->setText(QString("⚠️ <b>Attendance Recovery:</b> Student must attend the next <b>%1</b> consecutive class(es) to recover to %2% examination eligibility.").arg(QString::number(needed), QString::number(thresh)));
    } else {
        statusBadge->setText(QString("STATUS: DEBARRED FROM EXAMS (<65%: %1%)").arg(QString::number(pct, 'f', 1)));
        statusBadge->setStyleSheet("background-color: #FEF2F2; color: #DC2626; border: 1px solid #FECACA; padding: 8px 12px; border-radius: 6px; font-weight: 700;");

        double P = thresh / 100.0;
        int needed = static_cast<int>(std::ceil((P * total - attended) / (1.0 - P)));
        recoveryLabel->setText(QString("⛔ <b>Critical Action Required:</b> Student is currently DEBARRED. Must attend <b>%1</b> consecutive class(es) without absence to reach %2% eligibility.").arg(QString::number(needed), QString::number(thresh)));
    }
}

void AttendanceView::onSaveAttendance() {
    if (studentSelector->currentIndex() < 0) return;
    QString sId = studentSelector->currentData().toString();
    int att = attendedSpin->value();
    int tot = totalSpin->value();

    if (att > tot) {
        QMessageBox::warning(this, "Validation Error", "Attended classes cannot exceed total conducted classes.");
        return;
    }

    try {
        if (manager->setStudentAttendance(sId.toStdString(), att, tot)) {
            onThresholdChanged();
            applyTableFilter();
            emit dataModified();
            QMessageBox::information(this, "Attendance Updated", QString("Attendance updated for student %1.").arg(sId));
        }
    } catch (const exception& ex) {
        QMessageBox::warning(this, "Error", QString::fromStdString(ex.what()));
    }
}

void AttendanceView::applyTableFilter() {
    attendanceTable->setSortingEnabled(false);
    attendanceTable->setRowCount(0);

    const auto& students = manager->getAllStudents();
    QString filter = filterCombo->currentText();

    for (const auto& s : students) {
        double pct = s.getAttendancePercentage();
        if (filter.startsWith("Eligible") && pct < 75.0) continue;
        if (filter.startsWith("Warning") && (pct < 65.0 || pct >= 75.0)) continue;
        if (filter.startsWith("Debarred") && (pct >= 65.0 || s.getTotalClasses() == 0)) continue;

        int row = attendanceTable->rowCount();
        attendanceTable->insertRow(row);

        attendanceTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(s.getId())));
        attendanceTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(s.getName())));
        attendanceTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(s.getDepartment())));

        QTableWidgetItem* attItem = new QTableWidgetItem();
        attItem->setData(Qt::DisplayRole, s.getAttendedClasses());
        attendanceTable->setItem(row, 3, attItem);

        QTableWidgetItem* totItem = new QTableWidgetItem();
        totItem->setData(Qt::DisplayRole, s.getTotalClasses());
        attendanceTable->setItem(row, 4, totItem);

        QTableWidgetItem* pctItem = new QTableWidgetItem();
        pctItem->setData(Qt::DisplayRole, pct);
        pctItem->setText(QString::number(pct, 'f', 1) + "%");
        pctItem->setTextAlignment(Qt::AlignCenter);
        if (pct < 65.0) pctItem->setForeground(QBrush(QColor("#EF4444")));
        else if (pct < 75.0) pctItem->setForeground(QBrush(QColor("#F59E0B")));
        else pctItem->setForeground(QBrush(QColor("#10B981")));
        attendanceTable->setItem(row, 5, pctItem);

        QString statusStr = (s.getTotalClasses() == 0) ? "N/A" : (pct >= 75.0 ? "Eligible" : (pct >= 65.0 ? "Warning" : "Debarred"));
        QTableWidgetItem* statItem = new QTableWidgetItem(statusStr);
        statItem->setTextAlignment(Qt::AlignCenter);
        if (statusStr == "Eligible") statItem->setForeground(QBrush(QColor("#10B981")));
        else if (statusStr == "Warning") statItem->setForeground(QBrush(QColor("#F59E0B")));
        else if (statusStr == "Debarred") statItem->setForeground(QBrush(QColor("#EF4444")));
        attendanceTable->setItem(row, 6, statItem);
    }

    attendanceTable->setSortingEnabled(true);
}
