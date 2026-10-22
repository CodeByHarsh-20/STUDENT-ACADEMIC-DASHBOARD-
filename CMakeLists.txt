cmake_minimum_required(VERSION 3.16)
project(StudentAcademicDashboardGUI VERSION 2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt 6.7.2 installation path
set(CMAKE_PREFIX_PATH "C:/Qt/6.7.2/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets Charts)

# Source files: existing backend + new GUI
set(BACKEND_SOURCES
    Student.cpp
    Analytics.cpp
    FileManager.cpp
    AcademicManager.cpp
)

set(GUI_SOURCES
    gui_main.cpp
    gui/MainWindow.cpp
    gui/DashboardView.cpp
    gui/StudentsView.cpp
    gui/StudentDialog.cpp
    gui/MarksView.cpp
    gui/AttendanceView.cpp
    gui/AnalyticsView.cpp
    gui/MeritListView.cpp
    gui/CurriculumView.cpp
    gui/DataManagementView.cpp
)

set(HEADERS
    Person.h
    Student.h
    AcademicManager.h
    Analytics.h
    FileManager.h
    Exceptions.h
    dsa/BST.h
    dsa/Graph.h
    dsa/HashTable.h
    dsa/Heap.h
    dsa/LinkedList.h
    dsa/Queue.h
    dsa/Stack.h
    gui/StyleTheme.h
    gui/MainWindow.h
    gui/DashboardView.h
    gui/StudentsView.h
    gui/StudentDialog.h
    gui/MarksView.h
    gui/AttendanceView.h
    gui/AnalyticsView.h
    gui/MeritListView.h
    gui/CurriculumView.h
    gui/DataManagementView.h
)

qt_add_executable(StudentAcademicDashboardGUI
    ${BACKEND_SOURCES}
    ${GUI_SOURCES}
    ${HEADERS}
)

target_link_libraries(StudentAcademicDashboardGUI PRIVATE
    Qt6::Widgets
    Qt6::Charts
)

target_include_directories(StudentAcademicDashboardGUI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/gui
    ${CMAKE_CURRENT_SOURCE_DIR}/dsa
)

# Ensure the data directory exists in the build output
add_custom_command(TARGET StudentAcademicDashboardGUI POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:StudentAcademicDashboardGUI>/data"
    COMMENT "Creating data directory in build output"
)

# Copy existing students.txt to build directory
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/data/students.txt")
    add_custom_command(TARGET StudentAcademicDashboardGUI POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/data/students.txt"
            "$<TARGET_FILE_DIR:StudentAcademicDashboardGUI>/data/students.txt"
        COMMENT "Copying existing student records to build output"
    )
endif()

# Deploy Qt DLLs on Windows (windeployqt)
if(WIN32)
    add_custom_command(TARGET StudentAcademicDashboardGUI POST_BUILD
        COMMAND "C:/Qt/6.7.2/mingw_64/bin/windeployqt.exe"
            --no-translations
            --no-opengl-sw
            "$<TARGET_FILE:StudentAcademicDashboardGUI>"
        COMMENT "Running windeployqt to bundle Qt DLLs"
    )
endif()
