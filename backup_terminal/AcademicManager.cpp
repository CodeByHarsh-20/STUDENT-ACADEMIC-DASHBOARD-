#include <bits/stdc++.h>
#include "AcademicManager.h"
using namespace std;

AcademicManager::AcademicManager() {
    initializeCurriculumGraph();
    // Try loading existing records from data/students.txt
    try {
        if (fileManager.fileExists()) {
            loadRecords();
        } else {
            seedSampleDataIfEmpty();
        }
    } catch (const exception& ex) {
        cout << "[Notice] " << ex.what() << "\n";
        seedSampleDataIfEmpty();
    }
}

void AcademicManager::initializeCurriculumGraph() {
    // Course Definitions
    curriculumGraph.addCourse("CS101", "Programming in C++");
    curriculumGraph.addCourse("CS102", "Data Structures & Algorithms");
    curriculumGraph.addCourse("CS201", "Design & Analysis of Algorithms");
    curriculumGraph.addCourse("CS202", "Database Management Systems");
    curriculumGraph.addCourse("CS203", "Computer Architecture");
    curriculumGraph.addCourse("CS301", "Operating Systems");
    curriculumGraph.addCourse("CS302", "Computer Networks");
    curriculumGraph.addCourse("CS401", "Distributed Systems");
    curriculumGraph.addCourse("CS402", "Artificial Intelligence");

    // Prerequisite Edges (Prerequisite -> Dependent Course)
    curriculumGraph.addPrerequisite("CS101", "CS102"); // CS101 required for CS102
    curriculumGraph.addPrerequisite("CS102", "CS201"); // CS102 required for CS201
    curriculumGraph.addPrerequisite("CS102", "CS202"); // CS102 required for CS202
    curriculumGraph.addPrerequisite("CS101", "CS203"); // CS101 required for CS203
    curriculumGraph.addPrerequisite("CS203", "CS301"); // CS203 required for CS301
    curriculumGraph.addPrerequisite("CS301", "CS302"); // CS301 required for CS302
    curriculumGraph.addPrerequisite("CS301", "CS401"); // CS301 required for CS401
    curriculumGraph.addPrerequisite("CS202", "CS401"); // CS202 required for CS401
    curriculumGraph.addPrerequisite("CS201", "CS402"); // CS201 required for CS402
}

void AcademicManager::rebuildIndices() {
    idToIndexMap.clear();
    gpaIndexTree.clear();

    for (size_t i = 0; i < students.size(); ++i) {
        idToIndexMap.insert(students[i].getId(), i);
        // Use composite GPA key or student ID to keep BST nodes unique if necessary
        gpaIndexTree.insert(students[i].calculateGPA(), students[i].getId());
    }
}

void AcademicManager::seedSampleDataIfEmpty() {
    if (students.empty()) {
        cout << "[*] Loading pre-configured sample university records...\n";
        students = FileManager::generateSampleStudents();
        rebuildIndices();
        saveRecords();
        cout << "[+] Initialized " << students.size() << " sample students successfully.\n";
    }
}

void AcademicManager::pushUndoAction(const ActionCommand& cmd) {
    undoStack.push(cmd);
    redoStack.clear(); // Clear redo stack on new operation
}

bool AcademicManager::addStudent(const Student& s, bool recordUndo) {
    if (idToIndexMap.contains(s.getId())) {
        throw DuplicateStudentException(s.getId());
    }

    students.push_back(s);
    size_t newIndex = students.size() - 1;
    idToIndexMap.insert(s.getId(), newIndex);
    gpaIndexTree.insert(s.calculateGPA(), s.getId());

    if (recordUndo) {
        ActionCommand cmd;
        cmd.type = ActionType::ADD_STUDENT;
        cmd.currentState = s;
        cmd.description = "Added student " + s.getId() + " (" + s.getName() + ")";
        pushUndoAction(cmd);
    }
    return true;
}

void AcademicManager::viewAllStudents() const {
    if (students.empty()) {
        cout << "\n[!] No student records currently in database.\n";
        return;
    }

    cout << "\n========================================================================================================\n";
    cout << "                                  STUDENT ENROLLMENT DIRECTORY                                          \n";
    cout << "========================================================================================================\n";
    cout << left 
         << setw(12) << "Student ID" 
         << setw(22) << "Full Name" 
         << setw(8)  << "Sem" 
         << setw(10) << "Dept" 
         << setw(10) << "Score%" 
         << setw(8)  << "GPA" 
         << setw(12) << "Attend%" 
         << "Eligibility Status\n";
    cout << string(104, '-') << "\n";

    for (const auto& s : students) {
        cout << left 
             << setw(12) << s.getId() 
             << setw(22) << s.getName() 
             << setw(8)  << ("Sem " + to_string(s.getSemester()))
             << setw(10) << s.getDepartment() 
             << fixed << setprecision(1)
             << setw(10) << s.calculatePercentage() 
             << setw(8)  << s.calculateGPA() 
             << setw(12) << (to_string((int)s.getAttendancePercentage()) + "%")
             << s.getAttendanceStatus() << "\n";
    }
    cout << "========================================================================================================\n";
    cout << "Total Active Students: " << students.size() << "\n";
}

const Student* AcademicManager::searchStudentById(const string& id) const {
    // Fast O(1) Lookup via Hash Table
    const size_t* idxPtr = idToIndexMap.find(id);
    if (idxPtr != nullptr && *idxPtr < students.size()) {
        return &students[*idxPtr];
    }
    return nullptr;
}

vector<Student> AcademicManager::searchStudentsByName(const string& query) const {
    vector<Student> matches;
    string qLower = query;
    transform(qLower.begin(), qLower.end(), qLower.begin(), ::tolower);

    for (const auto& s : students) {
        string nameLower = s.getName();
        transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
        if (nameLower.find(qLower) != string::npos) {
            matches.push_back(s);
        }
    }
    return matches;
}

bool AcademicManager::updateStudentRecord(const string& id, const string& newName,
                                         int newAge, const string& newEmail,
                                         const string& newDept, int newSem) {
    size_t* idxPtr = idToIndexMap.find(id);
    if (!idxPtr || *idxPtr >= students.size()) {
        throw StudentNotFoundException(id);
    }

    Student& s = students[*idxPtr];
    Student previous = s;

    if (!newName.empty()) s.setName(newName);
    if (newAge > 0) s.setAge(newAge);
    if (!newEmail.empty()) s.setEmail(newEmail);
    if (!newDept.empty()) s.setDepartment(newDept);
    if (newSem >= 1 && newSem <= 8) s.setSemester(newSem);

    ActionCommand cmd;
    cmd.type = ActionType::UPDATE_STUDENT;
    cmd.previousState = previous;
    cmd.currentState = s;
    cmd.description = "Updated profile for student " + id;
    pushUndoAction(cmd);

    return true;
}

bool AcademicManager::deleteStudent(const string& id, bool recordUndo) {
    size_t* idxPtr = idToIndexMap.find(id);
    if (!idxPtr || *idxPtr >= students.size()) {
        throw StudentNotFoundException(id);
    }

    size_t index = *idxPtr;
    Student deleted = students[index];

    students.erase(students.begin() + index);
    rebuildIndices();

    if (recordUndo) {
        ActionCommand cmd;
        cmd.type = ActionType::DELETE_STUDENT;
        cmd.previousState = deleted;
        cmd.description = "Deleted student " + id + " (" + deleted.getName() + ")";
        pushUndoAction(cmd);
    }
    return true;
}

bool AcademicManager::setStudentMark(const string& id, const string& subject, double score) {
    size_t* idxPtr = idToIndexMap.find(id);
    if (!idxPtr || *idxPtr >= students.size()) {
        throw StudentNotFoundException(id);
    }

    Student& s = students[*idxPtr];
    Student previous = s;

    s.setMark(subject, score);
    rebuildIndices();

    ActionCommand cmd;
    cmd.type = ActionType::UPDATE_MARKS;
    cmd.previousState = previous;
    cmd.currentState = s;
    cmd.description = "Updated mark in " + subject + " for " + id + " to " + to_string((int)score);
    pushUndoAction(cmd);

    return true;
}

bool AcademicManager::setStudentAttendance(const string& id, int attended, int total) {
    size_t* idxPtr = idToIndexMap.find(id);
    if (!idxPtr || *idxPtr >= students.size()) {
        throw StudentNotFoundException(id);
    }

    Student& s = students[*idxPtr];
    Student previous = s;

    s.setAttendance(attended, total);

    ActionCommand cmd;
    cmd.type = ActionType::UPDATE_ATTENDANCE;
    cmd.previousState = previous;
    cmd.currentState = s;
    cmd.description = "Updated attendance for " + id + " to " + to_string(attended) + "/" + to_string(total);
    pushUndoAction(cmd);

    return true;
}

void AcademicManager::displayPerformanceAnalytics() const {
    Analytics::printAnalyticsReport(students);
}

void AcademicManager::generateMeritList(bool useBST) const {
    if (students.empty()) {
        cout << "\n[!] No students to rank.\n";
        return;
    }

    cout << "\n====================================================================================\n";
    cout << "                             OFFICIAL MERIT RANKING LIST                            \n";
    cout << "====================================================================================\n";
    cout << left 
         << setw(6)  << "Rank" 
         << setw(12) << "Student ID" 
         << setw(22) << "Student Name" 
         << setw(10) << "Dept" 
         << setw(10) << "Score%" 
         << setw(8)  << "GPA" 
         << "Letter Grade\n";
    cout << string(84, '-') << "\n";

    if (useBST) {
        // Merit Ranking generated using Custom Binary Search Tree Reverse In-Order Traversal
        // Time Complexity: O(N), Space: O(H) recursion stack
        int rank = 1;
        gpaIndexTree.reverseInOrderTraversal([this, &rank](const double& gpa, const string& studentId) {
            const Student* s = searchStudentById(studentId);
            if (s) {
                cout << left 
                     << setw(6)  << ("#" + to_string(rank++))
                     << setw(12) << s->getId() 
                     << setw(22) << s->getName() 
                     << setw(10) << s->getDepartment() 
                     << fixed << setprecision(1)
                     << setw(10) << s->calculatePercentage() 
                     << setw(8)  << s->calculateGPA() 
                     << s->calculateGrade() << "\n";
            }
        });
        cout << "------------------------------------------------------------------------------------\n";
        cout << "[Algorithm: Custom BST Reverse In-Order Traversal - O(N)]\n";
    } else {
        // Merit Ranking generated using QuickSort / MergeSort
        vector<Student> sortedStudents = students;
        Analytics::quickSortByGPA(sortedStudents, 0, (int)sortedStudents.size() - 1);

        for (size_t i = 0; i < sortedStudents.size(); ++i) {
            const auto& s = sortedStudents[i];
            cout << left 
                 << setw(6)  << ("#" + to_string(i + 1))
                 << setw(12) << s.getId() 
                 << setw(22) << s.getName() 
                 << setw(10) << s.getDepartment() 
                 << fixed << setprecision(1)
                 << setw(10) << s.calculatePercentage() 
                 << setw(8)  << s.calculateGPA() 
                 << s.calculateGrade() << "\n";
        }
        cout << "------------------------------------------------------------------------------------\n";
        cout << "[Algorithm: QuickSort - Average O(N log N)]\n";
    }
    cout << "====================================================================================\n";
}

void AcademicManager::generateAndDisplayAlerts() {
    if (students.empty()) {
        cout << "\n[!] No students to analyze for alerts.\n";
        return;
    }

    // Priority Heap for Alerts (DSA: Max-Heap)
    Heap<AcademicAlert, less<AcademicAlert>> alertHeap;

    for (const auto& s : students) {
        // 1. Debarred Attendance Alert (Priority 3 - Critical)
        if (s.getAttendancePercentage() < 65.0 && s.getTotalClasses() > 0) {
            alertHeap.push({3, s.getId(), s.getName(), 
                           "CRITICAL: Attendance is " + to_string((int)s.getAttendancePercentage()) + "% (DEBARRED from examinations)"});
        }
        // 2. Failing Grades / Arrears Alert (Priority 3 - Critical)
        if (s.hasFailingSubjects()) {
            string failedList = "";
            for (const auto& sub : s.getFailedSubjects()) {
                failedList += sub + " ";
            }
            alertHeap.push({3, s.getId(), s.getName(), 
                           "ACADEMIC ARREAR: Failed subject(s): " + failedList});
        }
        // 3. Attendance Warning Alert (Priority 2 - Warning)
        if (s.getAttendancePercentage() >= 65.0 && s.getAttendancePercentage() < 75.0) {
            alertHeap.push({2, s.getId(), s.getName(), 
                           "WARNING: Attendance is " + to_string((int)s.getAttendancePercentage()) + "% (Requires medical condonation)"});
        }
        // 4. Dean's Honor Roll (Priority 1 - Commendation)
        if (s.calculateGPA() >= 9.0 && !s.getMarks().empty()) {
            alertHeap.push({1, s.getId(), s.getName(), 
                           "HONORS: Outstanding GPA (" + to_string(s.calculateGPA()).substr(0, 4) + ") - Dean's List candidate"});
        }
    }

    cout << "\n====================================================================================\n";
    cout << "                STUDENT ACADEMIC & ATTENDANCE ALERTS DASHBOARD                      \n";
    cout << "====================================================================================\n";
    cout << "Total Active Alerts Identified: " << alertHeap.size() << "\n";
    cout << string(84, '-') << "\n";

    counselingQueue.clear();

    while (!alertHeap.empty()) {
        AcademicAlert alert = alertHeap.popValue();
        string tag = (alert.priority == 3) ? "[CRITICAL]" : (alert.priority == 2) ? "[WARNING] " : "[HONORS]  ";
        cout << left << setw(11) << tag 
             << setw(12) << alert.studentId 
             << setw(20) << alert.studentName 
             << alert.message << "\n";

        // Auto-enqueue critical alerts to Counseling FIFO Queue
        if (alert.priority >= 2) {
            counselingQueue.enqueue(alert);
        }
    }
    cout << "====================================================================================\n";
    cout << "Enqueued " << counselingQueue.size() << " alert(s) into University Counseling Queue.\n";
}

void AcademicManager::processNextCounselingAlert() {
    if (counselingQueue.empty()) {
        cout << "\n[v] University Counseling Queue is currently empty! All urgent alerts addressed.\n";
        return;
    }

    AcademicAlert alert = counselingQueue.dequeueValue();
    cout << "\n--- Processing Counseling Queue Item (FIFO) ---\n";
    cout << " Student ID   : " << alert.studentId << "\n";
    cout << " Student Name : " << alert.studentName << "\n";
    cout << " Alert Notice : " << alert.message << "\n";
    cout << " Status       : Dispatched academic counselor notification.\n";
    cout << " Remaining in Queue: " << counselingQueue.size() << "\n";
}

bool AcademicManager::undo() {
    if (undoStack.empty()) {
        cout << "\n[!] Undo stack is empty. No actions to undo.\n";
        return false;
    }

    ActionCommand cmd = undoStack.popValue();

    switch (cmd.type) {
        case ActionType::ADD_STUDENT: {
            // Reverse of Add is Delete
            deleteStudent(cmd.currentState.getId(), false);
            break;
        }
        case ActionType::DELETE_STUDENT: {
            // Reverse of Delete is Add
            addStudent(cmd.previousState, false);
            break;
        }
        case ActionType::UPDATE_STUDENT:
        case ActionType::UPDATE_MARKS:
        case ActionType::UPDATE_ATTENDANCE: {
            // Restore previous student state
            size_t* idxPtr = idToIndexMap.find(cmd.previousState.getId());
            if (idxPtr && *idxPtr < students.size()) {
                students[*idxPtr] = cmd.previousState;
                rebuildIndices();
            }
            break;
        }
    }

    redoStack.push(cmd);
    cout << "\n[<] Successfully Undone: " << cmd.description << "\n";
    return true;
}

bool AcademicManager::redo() {
    if (redoStack.empty()) {
        cout << "\n[!] Redo stack is empty. No actions to redo.\n";
        return false;
    }

    ActionCommand cmd = redoStack.popValue();

    switch (cmd.type) {
        case ActionType::ADD_STUDENT: {
            addStudent(cmd.currentState, false);
            break;
        }
        case ActionType::DELETE_STUDENT: {
            deleteStudent(cmd.previousState.getId(), false);
            break;
        }
        case ActionType::UPDATE_STUDENT:
        case ActionType::UPDATE_MARKS:
        case ActionType::UPDATE_ATTENDANCE: {
            size_t* idxPtr = idToIndexMap.find(cmd.currentState.getId());
            if (idxPtr && *idxPtr < students.size()) {
                students[*idxPtr] = cmd.currentState;
                rebuildIndices();
            }
            break;
        }
    }

    undoStack.push(cmd);
    cout << "\n[>] Successfully Redone: " << cmd.description << "\n";
    return true;
}

void AcademicManager::displayUndoRedoStatus() const {
    cout << "\n[Status] Undo Operations Available: " << undoStack.size()
         << " | Redo Operations Available: " << redoStack.size() << "\n";
}

void AcademicManager::checkSubjectPrerequisites(const string& studentId, const string& courseCode) const {
    const Student* s = searchStudentById(studentId);
    if (!s) {
        throw StudentNotFoundException(studentId);
    }

    cout << "\nChecking prerequisites for Course: " << courseCode 
         << " (" << curriculumGraph.getCourseName(courseCode) << ")...\n";
    cout << "Student: " << s->getName() << " (" << s->getId() << ")\n";

    vector<string> directReqs = curriculumGraph.getDirectPrerequisites(courseCode);
    cout << "Direct Prerequisites   : ";
    if (directReqs.empty()) {
        cout << "None (Open to all students)\n";
    } else {
        for (size_t i = 0; i < directReqs.size(); ++i) {
            cout << directReqs[i] << " (" << curriculumGraph.getCourseName(directReqs[i]) << ")"
                 << (i + 1 < directReqs.size() ? ", " : "\n");
        }
    }

    vector<string> allReqs = curriculumGraph.getAllPrerequisites(courseCode);
    cout << "All Prerequisite Tree (DFS): ";
    if (allReqs.empty()) {
        cout << "None\n";
    } else {
        for (size_t i = 0; i < allReqs.size(); ++i) {
            cout << allReqs[i] << (i + 1 < allReqs.size() ? ", " : "\n");
        }
    }

    vector<string> missing;
    bool eligible = curriculumGraph.checkEligibility(courseCode, s->getCompletedCourses(), missing);

    cout << "Registration Verdict    : ";
    if (eligible) {
        cout << "[ELIGIBLE] Student has completed all mandatory prerequisites!\n";
    } else {
        cout << "[NOT ELIGIBLE] Student is missing the following required course(s):\n";
        for (const auto& m : missing) {
            cout << "   - " << m << " : " << curriculumGraph.getCourseName(m) << "\n";
        }
    }
}

void AcademicManager::displayCurriculumGraph() const {
    curriculumGraph.displayCurriculum();
}

void AcademicManager::displayOptimalCourseSequence() const {
    cout << "\n================ OPTIMAL CURRICULUM SEQUENCE (TOPOLOGICAL ORDER) ================\n";
    cout << "Algorithm: Kahn's Algorithm (DAG In-Degree BFS) - O(V + E)\n";
    cout << "Cycle Detection: " << (curriculumGraph.hasCycle() ? "CYCLE DETECTED (Invalid DAG)" : "PASSED (Valid DAG)") << "\n\n";

    vector<string> topoOrder = curriculumGraph.getTopologicalOrder();
    for (size_t i = 0; i < topoOrder.size(); ++i) {
        cout << " Term " << (i + 1) << " -> [" << topoOrder[i] << "] " 
             << curriculumGraph.getCourseName(topoOrder[i]) << "\n";
    }
    cout << "=================================================================================\n";
}

bool AcademicManager::saveRecords() {
    try {
        fileManager.saveStudents(students);
        cout << "\n[+] Successfully saved " << students.size() 
             << " student record(s) to '" << fileManager.getFilePath() << "'.\n";
        return true;
    } catch (const exception& ex) {
        cerr << "\n[-] Error saving records: " << ex.what() << "\n";
        return false;
    }
}

bool AcademicManager::loadRecords() {
    try {
        vector<Student> loaded = fileManager.loadStudents();
        students = loaded;
        rebuildIndices();
        undoStack.clear();
        redoStack.clear();
        cout << "\n[+] Successfully loaded " << students.size() 
             << " student record(s) from '" << fileManager.getFilePath() << "'.\n";
        return true;
    } catch (const exception& ex) {
        cerr << "\n[-] Error loading records: " << ex.what() << "\n";
        return false;
    }
}
