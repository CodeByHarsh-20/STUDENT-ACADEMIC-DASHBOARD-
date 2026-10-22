#ifndef MERITLISTVIEW_H
#define MERITLISTVIEW_H

#include <QWidget>
#include <QTableWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include "../AcademicManager.h"

class MeritListView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;

    QComboBox* algorithmCombo;
    QComboBox* deptFilter;
    QTableWidget* meritTable;
    QLabel* complexityLabel;

    void setupUi();

public:
    explicit MeritListView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();

private slots:
    void generateMeritList();
};

#endif // MERITLISTVIEW_H
