#include <QApplication>
#include <QFont>
#include "gui/MainWindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Set application-wide font
    QFont appFont("Segoe UI", 10);
    app.setFont(appFont);

    app.setApplicationName("Student Academic Dashboard");
    app.setApplicationVersion("2.0");
    app.setOrganizationName("VIT University DSA Project");

    MainWindow window;
    window.show();

    return app.exec();
}
