#ifndef STYLETHEME_H
#define STYLETHEME_H

#include <QString>

namespace StyleTheme {
    // Primary Colors
    inline const char* COLOR_PRIMARY = "#2563EB";       // Royal Blue
    inline const char* COLOR_PRIMARY_HOVER = "#1D4ED8";
    inline const char* COLOR_NAVY_BG = "#0F172A";       // Slate 900
    inline const char* COLOR_NAVY_SURFACE = "#1E293B";  // Slate 800
    inline const char* COLOR_NAVY_HOVER = "#334155";    // Slate 700
    inline const char* COLOR_PAGE_BG = "#F8FAFC";       // Slate 50
    inline const char* COLOR_CARD_BG = "#FFFFFF";       // Pure White
    inline const char* COLOR_BORDER = "#E2E8F0";        // Slate 200
    inline const char* COLOR_BORDER_STRONG = "#CBD5E1"; // Slate 300
    inline const char* COLOR_TEXT_PRIMARY = "#0F172A";  // Slate 900
    inline const char* COLOR_TEXT_MUTED = "#64748B";    // Slate 500
    inline const char* COLOR_SUCCESS = "#10B981";       // Emerald 500
    inline const char* COLOR_WARNING = "#F59E0B";       // Amber 500
    inline const char* COLOR_DANGER = "#EF4444";        // Rose 500

    inline QString getGlobalStyleSheet() {
        return QString(R"(
            /* Global Application Typography and Resets */
            * {
                font-family: 'Segoe UI', -apple-system, BlinkMacSystemFont, 'Roboto', sans-serif;
                font-size: 13px;
                color: #0F172A;
            }

            QWidget#centralHost {
                background-color: #F8FAFC;
            }

            /* Scrollbars */
            QScrollBar:vertical {
                border: none;
                background: #F1F5F9;
                width: 8px;
                margin: 0px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical {
                background: #CBD5E1;
                min-height: 24px;
                border-radius: 4px;
            }
            QScrollBar::handle:vertical:hover {
                background: #94A3B8;
            }
            QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
                height: 0px;
            }
            QScrollBar:horizontal {
                border: none;
                background: #F1F5F9;
                height: 8px;
                margin: 0px;
                border-radius: 4px;
            }
            QScrollBar::handle:horizontal {
                background: #CBD5E1;
                min-width: 24px;
                border-radius: 4px;
            }

            /* Tables */
            QTableWidget {
                background-color: #FFFFFF;
                border: 1px solid #E2E8F0;
                border-radius: 8px;
                gridline-color: #F1F5F9;
                selection-background-color: #EFF6FF;
                selection-color: #1E3A8A;
                outline: none;
            }
            QTableWidget::item {
                padding: 10px 12px;
                border-bottom: 1px solid #F1F5F9;
            }
            QTableWidget::item:selected {
                background-color: #EFF6FF;
                color: #1E3A8A;
                font-weight: 600;
            }
            QHeaderView::section {
                background-color: #F8FAFC;
                color: #475569;
                padding: 10px 12px;
                font-weight: 700;
                font-size: 11px;
                text-transform: uppercase;
                letter-spacing: 0.5px;
                border: none;
                border-bottom: 2px solid #E2E8F0;
            }

            /* Form Inputs */
            QLineEdit, QComboBox, QSpinBox, QDoubleSpinBox {
                background-color: #FFFFFF;
                border: 1px solid #CBD5E1;
                border-radius: 6px;
                padding: 8px 12px;
                font-size: 13px;
                color: #0F172A;
            }
            QLineEdit:focus, QComboBox:focus, QSpinBox:focus, QDoubleSpinBox:focus {
                border: 2px solid #2563EB;
                background-color: #FFFFFF;
            }
            QComboBox::drop-down {
                subcontrol-origin: padding;
                subcontrol-position: top right;
                width: 24px;
                border-left-width: 0px;
                border-top-right-radius: 6px;
                border-bottom-right-radius: 6px;
            }
            QComboBox QAbstractItemView {
                background-color: #FFFFFF;
                border: 1px solid #CBD5E1;
                border-radius: 6px;
                selection-background-color: #EFF6FF;
                selection-color: #1E3A8A;
                padding: 4px;
            }

            /* Buttons */
            QPushButton {
                background-color: #2563EB;
                color: #FFFFFF;
                border: none;
                border-radius: 6px;
                padding: 8px 16px;
                font-weight: 600;
                font-size: 13px;
            }
            QPushButton:hover {
                background-color: #1D4ED8;
            }
            QPushButton:pressed {
                background-color: #1E40AF;
            }
            QPushButton:disabled {
                background-color: #CBD5E1;
                color: #94A3B8;
            }

            /* Outlined / Secondary Buttons */
            QPushButton.secondaryBtn {
                background-color: #FFFFFF;
                color: #334155;
                border: 1px solid #CBD5E1;
            }
            QPushButton.secondaryBtn:hover {
                background-color: #F1F5F9;
                border-color: #94A3B8;
            }

            /* Danger Button */
            QPushButton.dangerBtn {
                background-color: #EF4444;
                color: #FFFFFF;
            }
            QPushButton.dangerBtn:hover {
                background-color: #DC2626;
            }

            /* Cards */
            QFrame.statCard {
                background-color: #FFFFFF;
                border: 1px solid #E2E8F0;
                border-radius: 10px;
                padding: 16px;
            }

            /* Group Box */
            QGroupBox {
                background-color: #FFFFFF;
                border: 1px solid #E2E8F0;
                border-radius: 8px;
                margin-top: 18px;
                font-weight: 600;
                font-size: 13px;
                color: #334155;
                padding-top: 16px;
            }
            QGroupBox::title {
                subcontrol-origin: margin;
                subcontrol-position: top left;
                padding: 0 8px;
                background-color: transparent;
            }

            /* Tab Widget */
            QTabWidget::pane {
                border: 1px solid #E2E8F0;
                border-radius: 8px;
                background: #FFFFFF;
                top: -1px;
            }
            QTabBar::tab {
                background: #F1F5F9;
                color: #64748B;
                padding: 8px 16px;
                border-top-left-radius: 6px;
                border-top-right-radius: 6px;
                margin-right: 4px;
                font-weight: 600;
            }
            QTabBar::tab:selected {
                background: #FFFFFF;
                color: #2563EB;
                border: 1px solid #E2E8F0;
                border-bottom: 1px solid #FFFFFF;
            }
        )");
    }
}

#endif // STYLETHEME_H
