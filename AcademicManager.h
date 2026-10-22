#ifndef ACADEMICMANAGER_H
#define ACADEMICMANAGER_H

#include <bits/stdc++.h>
#include "Student.h"
#include "FileManager.h"
#include "Analytics.h"
#include "dsa/LinkedList.h"
#include "dsa/Stack.h"
#include "dsa/Queue.h"
#include "dsa/HashTable.h"
#include "dsa/BST.h"
#include "dsa/Graph.h"
#include "dsa/Heap.h"
#include "Exceptions.h"
using namespace std;

// Command Pattern for Undo / Redo
enum class ActionType {
    ADD_STUDENT,
    DELETE_STUDENT,
    UPDATE_STUDENT,
    UPDATE_MARKS,
    UPDATE_ATTENDANCE
};

struct ActionCommand {
    ActionType type;
    Student previousState;
    Student currentState;
    string description;
};

// Priority Alert Item
struct AcademicAlert {
    int priority; // 3 = Critical (Debarred/Arrear), 2 = Warning (Low Attendance), 1 = Honor (Dean's List)
    string studentId;
    string studentName;
    string message;

    bool operator<(const AcademicAlert& other) const {
        return priority < other.priority; // higher priority first in Max-Heap
    }
    bool operator>(const AcademicAlert& other) const {
        return priority > other.priority;
    }
};

class AcademicManager {
private:
    vector<Student> students;                          // Sequential record store
    HashTable<string, size_t> idToIndexMap;            // Fast O(1) ID -> vector index lookup
    BST<double, string> gpaIndexTree;                  // BST indexed by GPA for O(log N) merit queries
    CourseGraph curriculumGraph;                       // Prerequisite DAG
    FileManager fileManager;                           // File persistence manager

    // Undo / Redo Stacks (DSA: Stacks)
    Stack<ActionCommand> undoStack;
    Stack<ActionCommand> redoStack;

    // Queue for Counseling / Review (DSA: Queue)
    Queue<AcademicAlert> counselingQueue;

    void rebuildIndices();
    void initializeCurriculumGraph();
    void pushUndoAction(const ActionCommand& cmd);

public:
    AcademicManager();
    ~AcademicManager() = default;

    // 1. Add Student
    bool addStudent(const Student& s, bool recordUndo = true);

    // 2. View All Students
    void viewAllStudents() const;

    // 3. Search Student (Hash Table O(1) by ID or Linear by Name)
    const Student* searchStudentById(const string& id) const;
    vector<Student> searchStudentsByName(const string& query) const;

    // 4. Update Student Record
    bool updateStudentRecord(const string& id, const string& newName,
                            int newAge, const string& newEmail,
                            const string& newDept, int newSem);

    // 5. Delete Student
    bool deleteStudent(const string& id, bool recordUndo = true);

    // 6. Manage Marks
    bool setStudentMark(const string& id, const string& subject, double score);

    // 7. Manage Attendance
    bool setStudentAttendance(const string& id, int attended, int total);

    // 8. Performance Analytics
    void displayPerformanceAnalytics() const;

    // 9. Generate Merit List
    void generateMeritList(bool useBST = true) const;

    // 10. Student Alerts
    void generateAndDisplayAlerts();
    void processNextCounselingAlert();

    // 11. Undo / Redo
    bool undo();
    bool redo();
    void displayUndoRedoStatus() const;

    // 12. Subject Prerequisite Checker
    void checkSubjectPrerequisites(const string& studentId, const string& courseCode) const;
    void displayCurriculumGraph() const;
    void displayOptimalCourseSequence() const;

    // 13. Save Records
    bool saveRecords();

    // 14. Load Records
    bool loadRecords();

    // Utility Helpers & GUI Accessors
    size_t getStudentCount() const { return students.size(); }
    void seedSampleDataIfEmpty();
    const vector<Student>& getAllStudents() const { return students; }
    vector<Student> getMeritListBST() const;
    vector<Student> getMeritListQuickSort() const;
    vector<AcademicAlert> generateAlertsFromHeap();
    const CourseGraph& getCurriculumGraph() const { return curriculumGraph; }
    size_t getCounselingQueueSize() const { return counselingQueue.size(); }
    bool hasCounselingAlerts() const { return !counselingQueue.empty(); }
    bool processNextCounselingAlertItem(AcademicAlert& alert);
    size_t getUndoCount() const { return undoStack.size(); }
    size_t getRedoCount() const { return redoStack.size(); }
    string getUndoDescription() const;
    string getRedoDescription() const;
};

#endif // ACADEMICMANAGER_H
