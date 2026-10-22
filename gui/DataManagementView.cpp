#include "DataManagementView.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QFrame>
#include <QFileDialog>

DataManagementView::DataManagementView(AcademicManager* mgr, QWidget* parent)
    : QWidget(parent), manager(mgr) {
    setupUi();
    refreshData();
}

void DataManagementView::setupUi() {
    QVBoxLayout* root = new QVBoxLayout(this);
    root->setContentsMargins(24, 24, 24, 24);
    root->setSpacing(20);

    QLabel* pageTitle = new QLabel("Data Management & Persistence", this);
    pageTitle->setStyleSheet("font-size: 22px; font-weight: 700; color: #0F172A;");
    QLabel* pageSubtitle = new QLabel("Manage academic record persistence. Records are stored in 'data/students.txt' using the project's serialization format.", this);
    pageSubtitle->setStyleSheet("font-size: 13px; color: #64748B;");
    root->addWidget(pageTitle);
    root->addWidget(pageSubtitle);

    // Action Cards
    auto makeActionCard = [this](const QString& title, const QString& desc,
                                  const QString& btnLabel, const QString& btnStyle,
                                  const char* slot) -> QFrame* {
        QFrame* card = new QFrame(this);
        card->setStyleSheet("background-color: #FFFFFF; border: 1px solid #E2E8F0; border-radius: 10px; padding: 16px;");
        QVBoxLayout* cl = new QVBoxLayout(card);
        cl->setSpacing(10);

        QLabel* t = new QLabel(title, card);
        t->setStyleSheet("font-size: 15px; font-weight: 700; color: #0F172A; border: none;");

        QLabel* d = new QLabel(desc, card);
        d->setWordWrap(true);
        d->setStyleSheet("color: #475569; font-size: 12px; border: none;");

        QPushButton* btn = new QPushButton(btnLabel, card);
        btn->setStyleSheet(btnStyle);
        btn->setFixedWidth(200);
        connect(btn, SIGNAL(clicked()), this, slot);

        cl->addWidget(t);
        cl->addWidget(d);
        cl->addWidget(btn, 0, Qt::AlignRight);
        return card;
    };

    root->addWidget(makeActionCard(
        "💾  Save Records to Disk",
        "Serializes all current in-memory student records to 'data/students.txt' using the project's pipe-delimited format (ID|Name|Age|Email|Dept|Sem|Attended|Total|Marks|Courses). This replaces the current file contents.",
        "Save All Records",
        "background-color: #2563EB; color: white; padding: 8px 16px; border-radius: 6px; font-weight: 600;",
        SLOT(onSave())
    ));

    root->addWidget(makeActionCard(
        "📂  Reload Records from Disk",
        "Loads and deserializes all records from 'data/students.txt'. Corrupted or malformed lines are automatically skipped with a warning. Existing in-memory records are replaced. The Undo/Redo history is cleared after loading.",
        "Reload from File",
        "background-color: #FFFFFF; color: #475569; border: 1px solid #CBD5E1; padding: 8px 16px; border-radius: 6px; font-weight: 600;",
        SLOT(onLoad())
    ));

    root->addWidget(makeActionCard(
        "🧪  Initialize with Sample Student Data",
        "Clears the current database and pre-populates it with 6 realistic sample students across CSE, ECE, MECH, and IT departments with varied attendance and academic performance — ideal for evaluation and testing.",
        "Load Sample Data",
        "background-color: #FFFBEB; color: #92400E; border: 1px solid #FDE68A; padding: 8px 16px; border-radius: 6px; font-weight: 600;",
        SLOT(onSeedSample())
    ));

    statusLabel = new QLabel("", this);
    statusLabel->setWordWrap(true);
    statusLabel->setStyleSheet("padding: 10px 14px; background-color: #F8FAFC; border: 1px solid #E2E8F0; border-radius: 6px; color: #475569;");
    root->addWidget(statusLabel);

    root->addStretch();
}

void DataManagementView::refreshData() {
    statusLabel->setText(QString("Current database: %1 student record(s) loaded in memory.")
                         .arg(manager->getStudentCount()));
}

void DataManagementView::onSave() {
    bool ok = manager->saveRecords();
    statusLabel->setText(ok
        ? QString("✅ Saved %1 student record(s) to 'data/students.txt' successfully.").arg(manager->getStudentCount())
        : "❌ Failed to save records. Check that the 'data/' directory exists and is writable.");
    statusLabel->setStyleSheet(ok
        ? "padding: 10px 14px; background-color: #ECFDF5; border: 1px solid #A7F3D0; border-radius: 6px; color: #065F46; font-weight: 600;"
        : "padding: 10px 14px; background-color: #FEF2F2; border: 1px solid #FECACA; border-radius: 6px; color: #7F1D1D; font-weight: 600;");
    if (ok) emit dataModified();
}

void DataManagementView::onLoad() {
    auto reply = QMessageBox::question(this, "Reload Records",
        "This will replace all in-memory student records with the saved file contents and clear the Undo/Redo history.\n\nContinue?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    bool ok = manager->loadRecords();
    statusLabel->setText(ok
        ? QString("✅ Loaded %1 student record(s) from file successfully.").arg(manager->getStudentCount())
        : "❌ Failed to load records. Ensure 'data/students.txt' exists.");
    statusLabel->setStyleSheet(ok
        ? "padding: 10px 14px; background-color: #ECFDF5; border: 1px solid #A7F3D0; border-radius: 6px; color: #065F46; font-weight: 600;"
        : "padding: 10px 14px; background-color: #FEF2F2; border: 1px solid #FECACA; border-radius: 6px; color: #7F1D1D; font-weight: 600;");
    if (ok) emit dataModified();
}

void DataManagementView::onSeedSample() {
    auto reply = QMessageBox::question(this, "Load Sample Data",
        "This will REPLACE all existing student records with 6 pre-configured sample students.\n\nContinue?",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply != QMessageBox::Yes) return;

    manager->seedSampleDataIfEmpty();
    emit dataModified();
    statusLabel->setText(QString("✅ Sample data loaded. Database now contains %1 student record(s).").arg(manager->getStudentCount()));
    statusLabel->setStyleSheet("padding: 10px 14px; background-color: #ECFDF5; border: 1px solid #A7F3D0; border-radius: 6px; color: #065F46; font-weight: 600;");
}
