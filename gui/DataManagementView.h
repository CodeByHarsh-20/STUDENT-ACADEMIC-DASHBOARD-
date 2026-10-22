#ifndef DATAMANAGEMENTVIEW_H
#define DATAMANAGEMENTVIEW_H

#include <QWidget>
#include <QLabel>
#include "../AcademicManager.h"

class DataManagementView : public QWidget {
    Q_OBJECT
private:
    AcademicManager* manager;
    QLabel* statusLabel;
    void setupUi();

public:
    explicit DataManagementView(AcademicManager* mgr, QWidget* parent = nullptr);
    void refreshData();

signals:
    void dataModified();

private slots:
    void onSave();
    void onLoad();
    void onSeedSample();
};

#endif // DATAMANAGEMENTVIEW_H
