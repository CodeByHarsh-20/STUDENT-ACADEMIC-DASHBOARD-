#include <bits/stdc++.h>
#include "AcademicManager.h"
#include "Exceptions.h"
using namespace std;

// Helper for safe integer inputs
int readInt(const string& prompt, int minVal = numeric_limits<int>::min(), int maxVal = numeric_limits<int>::max()) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if (val >= minVal && val <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return val;
            } else {
                cout << "Invalid range! Must be between " << minVal << " and " << maxVal << ". Try again.\n";
            }
        } else {
            cout << "Invalid numeric input! Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Helper for safe double inputs
double readDouble(const string& prompt, double minVal = 0.0, double maxVal = 100.0) {
    double val;
    while (true) {
        cout << prompt;
        if (cin >> val) {
            if (val >= minVal && val <= maxVal) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return val;
            } else {
                cout << "Invalid range! Must be between " << minVal << " and " << maxVal << ". Try again.\n";
            }
        } else {
            cout << "Invalid numeric input! Please enter a valid number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

// Helper for string lines
string readLine(const string& prompt) {
    string line;
    cout << prompt;
    getline(cin, line);
    return line;
}

void printBanner() {
    cout << "\n========================================================\n";
    cout << "      UNIVERSITY ACADEMIC DASHBOARD & MANAGEMENT SYSTEM \n";
    cout << "========================================================\n";
}

void printMenu() {
    cout << "\n========== MAIN MENU ==========\n\n";
    cout << "1. Add Student\n";
    cout << "2. View All Students\n";
    cout << "3. Search Student\n";
    cout << "4. Update Student Record\n";
    cout << "5. Delete Student\n";
    cout << "6. Manage Marks\n";
    cout << "7. Manage Attendance\n";
    cout << "8. Performance Analytics\n";
    cout << "9. Generate Merit List\n";
    cout << "10. Student Alerts\n";
    cout << "11. Undo / Redo Changes\n";
    cout << "12. Subject Prerequisite Checker\n";
    cout << "13. Save Records\n";
    cout << "14. Load Records\n";
    cout << "0. Exit\n\n";
}

int main() {
    printBanner();
    AcademicManager manager;

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Enter your choice: ", 0, 14);

        try {
            switch (choice) {
                case 1: { // 1. Add Student
                    cout << "\n--- ADD NEW STUDENT RECORD ---\n";
                    string id = readLine("Enter Student ID (e.g., 24BCE1007): ");
                    if (manager.searchStudentById(id) != nullptr) {
                        cout << "[!] Error: Student with ID '" << id << "' already exists!\n";
                        break;
                    }
                    string name = readLine("Enter Student Full Name: ");
                    int age = readInt("Enter Age (16-99): ", 16, 99);
                    string email = readLine("Enter Email Address: ");
                    string dept = readLine("Enter Department (e.g., CSE, ECE, MECH): ");
                    int sem = readInt("Enter Semester (1-8): ", 1, 8);

                    Student newStudent(id, name, age, email, dept, sem);

                    // Optional initial attendance
                    char attChoice;
                    cout << "Do you want to enter attendance now? (y/n): ";
                    cin >> attChoice;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (attChoice == 'y' || attChoice == 'Y') {
                        int total = readInt("Enter Total Classes Conducted: ", 1, 200);
                        int attended = readInt("Enter Classes Attended: ", 0, total);
                        newStudent.setAttendance(attended, total);
                    }

                    manager.addStudent(newStudent);
                    cout << "\n[+] Student '" << name << "' (" << id << ") successfully enrolled!\n";
                    break;
                }

                case 2: { // 2. View All Students
                    manager.viewAllStudents();
                    break;
                }

                case 3: { // 3. Search Student
                    cout << "\n--- SEARCH STUDENT ---\n";
                    cout << "1. Search by Student ID (O(1) Hash Table Lookup)\n";
                    cout << "2. Search by Name (Substring Pattern Match)\n";
                    int sChoice = readInt("Enter search option (1-2): ", 1, 2);

                    if (sChoice == 1) {
                        string id = readLine("Enter Student ID to find: ");
                        const Student* s = manager.searchStudentById(id);
                        if (s) {
                            cout << "\n[+] Student Found! (Hash Table O(1) Search)\n";
                            s->displayDetailedReport();
                        } else {
                            cout << "\n[-] No student found with ID: " << id << "\n";
                        }
                    } else {
                        string name = readLine("Enter Name or partial query: ");
                        auto matches = manager.searchStudentsByName(name);
                        if (matches.empty()) {
                            cout << "\n[-] No students matching query '" << name << "' found.\n";
                        } else {
                            cout << "\n[+] Found " << matches.size() << " matching record(s):\n";
                            for (const auto& s : matches) {
                                s.displayDetailedReport();
                            }
                        }
                    }
                    break;
                }

                case 4: { // 4. Update Student Record
                    cout << "\n--- UPDATE STUDENT PROFILE ---\n";
                    string id = readLine("Enter Student ID to update: ");
                    const Student* s = manager.searchStudentById(id);
                    if (!s) {
                        cout << "[-] Student ID '" << id << "' not found.\n";
                        break;
                    }
                    cout << "Current record: " << *s << "\n";
                    cout << "Press Enter to keep current value:\n";

                    string newName = readLine("New Name [" + s->getName() + "]: ");
                    string ageStr = readLine("New Age [" + to_string(s->getAge()) + "]: ");
                    int newAge = ageStr.empty() ? s->getAge() : stoi(ageStr);

                    string newEmail = readLine("New Email [" + s->getEmail() + "]: ");
                    string newDept = readLine("New Department [" + s->getDepartment() + "]: ");
                    string semStr = readLine("New Semester [" + to_string(s->getSemester()) + "]: ");
                    int newSem = semStr.empty() ? s->getSemester() : stoi(semStr);

                    manager.updateStudentRecord(id, 
                        newName.empty() ? s->getName() : newName,
                        newAge,
                        newEmail.empty() ? s->getEmail() : newEmail,
                        newDept.empty() ? s->getDepartment() : newDept,
                        newSem);

                    cout << "[+] Student record updated successfully!\n";
                    break;
                }

                case 5: { // 5. Delete Student
                    cout << "\n--- DELETE STUDENT RECORD ---\n";
                    string id = readLine("Enter Student ID to remove: ");
                    const Student* s = manager.searchStudentById(id);
                    if (!s) {
                        cout << "[-] Student ID '" << id << "' not found.\n";
                        break;
                    }

                    cout << "Are you sure you want to delete [" << id << "] " << s->getName() << "? (y/n): ";
                    char confirm;
                    cin >> confirm;
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    if (confirm == 'y' || confirm == 'Y') {
                        manager.deleteStudent(id);
                        cout << "[+] Student record deleted. (Action can be Undone)\n";
                    } else {
                        cout << "[*] Deletion canceled.\n";
                    }
                    break;
                }

                case 6: { // 6. Manage Marks
                    cout << "\n--- MANAGE SUBJECT MARKS ---\n";
                    string id = readLine("Enter Student ID: ");
                    const Student* s = manager.searchStudentById(id);
                    if (!s) {
                        cout << "[-] Student ID '" << id << "' not found.\n";
                        break;
                    }

                    cout << "1. Add or Update Subject Mark\n";
                    cout << "2. View Subject Marks\n";
                    int mChoice = readInt("Choose action (1-2): ", 1, 2);

                    if (mChoice == 1) {
                        string subject = readLine("Enter Subject Name (e.g., Data Structures, Operating Systems): ");
                        double score = readDouble("Enter Score / 100: ", 0.0, 100.0);
                        manager.setStudentMark(id, subject, score);
                        cout << "[+] Mark recorded! New Student GPA: " 
                             << fixed << setprecision(2) 
                             << manager.searchStudentById(id)->calculateGPA() << "\n";
                    } else {
                        s->displayDetailedReport();
                    }
                    break;
                }

                case 7: { // 7. Manage Attendance
                    cout << "\n--- MANAGE ATTENDANCE ---\n";
                    string id = readLine("Enter Student ID: ");
                    const Student* s = manager.searchStudentById(id);
                    if (!s) {
                        cout << "[-] Student ID '" << id << "' not found.\n";
                        break;
                    }

                    cout << "Current Attendance: " << s->getAttendedClasses() << "/" 
                         << s->getTotalClasses() << " (" << s->getAttendancePercentage() << "%)\n";
                    int total = readInt("Enter Total Classes Conducted: ", 1, 500);
                    int attended = readInt("Enter Classes Attended: ", 0, total);

                    manager.setStudentAttendance(id, attended, total);
                    const Student* updated = manager.searchStudentById(id);
                    cout << "[+] Attendance Updated! Status: " << updated->getAttendanceStatus() << "\n";
                    break;
                }

                case 8: { // 8. Performance Analytics
                    manager.displayPerformanceAnalytics();
                    break;
                }

                case 9: { // 9. Generate Merit List
                    cout << "\n--- GENERATE MERIT RANKING LIST ---\n";
                    cout << "1. Custom BST Reverse In-Order Traversal - O(N)\n";
                    cout << "2. Custom QuickSort by GPA - Average O(N log N)\n";
                    int sortChoice = readInt("Choose ranking algorithm (1-2): ", 1, 2);
                    manager.generateMeritList(sortChoice == 1);
                    break;
                }

                case 10: { // 10. Student Alerts
                    cout << "\n--- STUDENT ALERTS DASHBOARD (PRIORITY QUEUE) ---\n";
                    manager.generateAndDisplayAlerts();

                    cout << "\nCounseling Queue Action:\n";
                    cout << "1. Process Next Counseling Alert (FIFO Queue)\n";
                    cout << "2. Return to Main Menu\n";
                    int aChoice = readInt("Choose option (1-2): ", 1, 2);
                    if (aChoice == 1) {
                        manager.processNextCounselingAlert();
                    }
                    break;
                }

                case 11: { // 11. Undo / Redo Changes
                    cout << "\n--- UNDO / REDO COMMAND HISTORY (STACK) ---\n";
                    manager.displayUndoRedoStatus();
                    cout << "1. Undo Last Change (Stack Pop)\n";
                    cout << "2. Redo Next Change (Stack Push/Pop)\n";
                    cout << "3. Back to Main Menu\n";
                    int uChoice = readInt("Choose option (1-3): ", 1, 3);
                    if (uChoice == 1) {
                        manager.undo();
                    } else if (uChoice == 2) {
                        manager.redo();
                    }
                    break;
                }

                case 12: { // 12. Subject Prerequisite Checker
                    cout << "\n--- SUBJECT PREREQUISITE CHECKER (DIRECTED GRAPH) ---\n";
                    cout << "1. Check Student Eligibility for Course (Prerequisite DFS/BFS)\n";
                    cout << "2. View Full University Curriculum Graph & Prerequisites\n";
                    cout << "3. View Optimal Learning Order (Kahn's Topological Sort)\n";
                    int pChoice = readInt("Choose option (1-3): ", 1, 3);

                    if (pChoice == 1) {
                        string id = readLine("Enter Student ID: ");
                        const Student* s = manager.searchStudentById(id);
                        if (!s) {
                            cout << "[-] Student ID '" << id << "' not found.\n";
                            break;
                        }
                        cout << "\nAvailable Curriculum Courses:\n";
                        cout << "CS101, CS102, CS201, CS202, CS203, CS301, CS302, CS401, CS402\n";
                        string course = readLine("Enter Course Code to verify: ");
                        manager.checkSubjectPrerequisites(id, course);
                    } else if (pChoice == 2) {
                        manager.displayCurriculumGraph();
                    } else {
                        manager.displayOptimalCourseSequence();
                    }
                    break;
                }

                case 13: { // 13. Save Records
                    manager.saveRecords();
                    break;
                }

                case 14: { // 14. Load Records
                    manager.loadRecords();
                    break;
                }

                case 0: { // 0. Exit
                    cout << "\n[*] Auto-saving database records before exit...\n";
                    manager.saveRecords();
                    cout << "[+] All records persisted safely.\n";
                    cout << "Thank you for using the University Academic Management System!\n";
                    running = false;
                    break;
                }

                default:
                    cout << "[!] Invalid selection. Please choose an option from the menu.\n";
                    break;
            }
        } catch (const AcademicException& aEx) {
            cerr << "\n[Academic System Exception] " << aEx.what() << "\n";
        } catch (const exception& ex) {
            cerr << "\n[System Error] " << ex.what() << "\n";
        }
    }

    return 0;
}
