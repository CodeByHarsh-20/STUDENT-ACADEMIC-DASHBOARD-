# Student Academic Dashboard & University Academic Management System

A high-performance **University Academic Management System** built with **Modern C++** and **Custom Data Structures & Algorithms (DSA)**.

This project was built to satisfy all requirements of academic evaluation, demonstrating both advanced **C++ Object-Oriented Software Engineering** and fundamental **Data Structures & Algorithms**.

---

## Key Highlights & Concepts Demonstrated

### 1. C++ Concepts Demonstrated
- **Classes & Objects**: Structured domain models with clear separation of declarations (`.h`) and implementations (`.cpp`).
- **Inheritance & Polymorphism**: Abstract base class `Person` with pure virtual methods (`displaySummary()`) and virtual destructor, extended by derived class `Student`.
- **Encapsulation & Data Validation**: Private member variables with validated getters and setters (marks strictly `[0.0, 100.0]`, attendance `[0, total]`, semester `[1, 8]`).
- **Templates (Generic Programming)**: Entire `dsa/` directory features generic template classes: `LinkedList<T>`, `Stack<T>`, `Queue<T>`, `HashTable<K, V>`, `BST<Key, Value>`, `Heap<T, Comparator>`.
- **Exception Handling**: Custom exception hierarchy rooted at `AcademicException : public std::runtime_error`, including `StudentNotFoundException`, `DuplicateStudentException`, `InvalidInputException`, `DataStructureException`, and `FileIOException`.
- **File Handling & Serialization**: Persistent text file storage (`data/students.txt`) with custom serialization (`|` and `;` delimited tokens), automated error recovery for corrupted records, and automatic save on exit.
- **Operator Overloading**: Overloaded `operator<`, `operator>`, `operator==`, and `operator<<` for formatted console streams.
- **Modern C++ Standards**: Clean header guards, const-correctness, pass-by-reference semantics, functor comparators, and clean RAII memory management.

---

### 2. Data Structures & Algorithms (DSA) Demonstrated

| Data Structure / Algorithm | Source File | Where & How It Is Used | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- | :--- |
| **Hash Table** (Separate Chaining) | `dsa/HashTable.h` | Fast student ID lookup (`searchStudentById`, uniqueness verification) | Average: $\mathcal{O}(1)$<br>Worst: $\mathcal{O}(N)$ | $\mathcal{O}(M + N)$ |
| **Binary Search Tree (BST)** | `dsa/BST.h` | Merit list ranking by GPA, sorted range queries, in-order traversal | Search/Insert: $\mathcal{O}(\log N)$<br>Traversal: $\mathcal{O}(N)$ | $\mathcal{O}(N)$ |
| **Stack** (LIFO) | `dsa/Stack.h` | Command Pattern for **Undo** and **Redo** operations (push/pop actions) | Push/Pop: $\mathcal{O}(1)$ | $\mathcal{O}(K)$ |
| **Queue** (FIFO Linked Nodes) | `dsa/Queue.h` | University Counseling Dispatch Queue for sequential alert processing | Enqueue/Dequeue: $\mathcal{O}(1)$ | $\mathcal{O}(N)$ |
| **Min/Max Binary Heap** | `dsa/Heap.h` | Priority Queue for Student Alerts and Top-$K$ student ranking | Push/Pop: $\mathcal{O}(\log N)$<br>Top: $\mathcal{O}(1)$ | $\mathcal{O}(N)$ |
| **Directed Graph (DAG)** | `dsa/Graph.h` | Subject Prerequisite Checker with BFS/DFS and cycle detection | Traversal: $\mathcal{O}(V + E)$ | $\mathcal{O}(V + E)$ |
| **Kahn's Topological Sort** | `dsa/Graph.h` | Computes optimal sequence of university course completion | $\mathcal{O}(V + E)$ | $\mathcal{O}(V)$ |
| **QuickSort & MergeSort** | `Analytics.cpp` | In-place partitioning and divide-and-conquer sorting for Merit List | QuickSort: Avg $\mathcal{O}(N \log N)$<br>MergeSort: $\mathcal{O}(N \log N)$ | QuickSort: $\mathcal{O}(\log N)$<br>MergeSort: $\mathcal{O}(N)$ |
| **Binary Search** | `Analytics.cpp` | Logarithmic search on sorted student ID collections | $\mathcal{O}(\log N)$ | $\mathcal{O}(1)$ |
| **Linked List** | `dsa/LinkedList.h` | Custom dynamic sequential list with deep copying and predicate filters | Insertion: $\mathcal{O}(1)$<br>Search: $\mathcal{O}(N)$ | $\mathcal{O}(N)$ |

---

## Project Folder Structure

```
StudentAcademicDashboard/
│
├── main.cpp                  # Interactive CLI entry point and menu router
├── Person.h                  # Abstract base class demonstrating OOP inheritance
├── Student.h                 # Student derived class declaration
├── Student.cpp               # Student implementation, GPA calculation, serialization
├── AcademicManager.h         # Core coordinator integrating DSA and business logic
├── AcademicManager.cpp       # Workflow operations (Search, Update, Undo/Redo, Alerts)
├── Analytics.h               # Analytics & statistical calculations header
├── Analytics.cpp             # Mean, StdDev, Histogram, QuickSort, MergeSort, Top-K
├── FileManager.h             # File I/O persistence header
├── FileManager.cpp           # Serialization, load, save, sample seed data
├── Exceptions.h              # Custom exception hierarchy
│
├── dsa/                      # Custom Generic Data Structure Implementations
│   ├── LinkedList.h          # Singly Linked List template
│   ├── Stack.h               # LIFO Stack template for Undo / Redo
│   ├── Queue.h               # FIFO Queue template for Counseling Queue
│   ├── HashTable.h           # Hash Table with separate chaining & dynamic rehash
│   ├── BST.h                 # Binary Search Tree template with recursive traversals
│   ├── Graph.h               # Directed Graph for Course Prerequisites & Kahn's Algo
│   └── Heap.h                # Binary Max/Min Heap template for Top-K & Priority Alerts
│
├── data/
│   └── students.txt          # Persistent database file
│
└── README.md                 # Complete project documentation
```

---

## 14-Option Interactive Menu Breakdown

```
========== MAIN MENU ==========

1. Add Student
2. View All Students
3. Search Student
4. Update Student Record
5. Delete Student
6. Manage Marks
7. Manage Attendance
8. Performance Analytics
9. Generate Merit List
10. Student Alerts
11. Undo / Redo Changes
12. Subject Prerequisite Checker
13. Save Records
14. Load Records
0. Exit
```

1. **Add Student**: Enrolls a new student. Enforces unique student ID using the $\mathcal{O}(1)$ Hash Table and validates all inputs.
2. **View All Students**: Prints formatted directory containing ID, Name, Semester, Department, Overall Score %, 10-point GPA, Attendance %, and Exam Eligibility.
3. **Search Student**: 
   - Option A: Instant $\mathcal{O}(1)$ lookup by Student ID using `HashTable`.
   - Option B: Case-insensitive substring search by Name.
4. **Update Student Record**: Modifies profile information (Name, Age, Email, Dept, Semester). Creates an Undo snapshot.
5. **Delete Student**: Deletes student from the database. Rebuilds indices and enables full Undo.
6. **Manage Marks**: Add/update marks for subjects (0.0 to 100.0). Recalculates GPA (10-point scale) and letter grades.
7. **Manage Attendance**: Records attended vs conducted classes. Calculates attendance percentage and determines exam eligibility:
   - $\ge 75\%$: **ELIGIBLE**
   - $65\% - 74\%$: **WARNING** (Medical condonation required)
   - $< 65\%$: **DEBARRED**
8. **Performance Analytics**: Computes institutional metrics:
   - Class average percentage & average GPA
   - Highest and lowest scores
   - **Standard Deviation** (variance calculation)
   - Pass percentage ($\ge 40\%$)
   - Attendance compliance breakdown
   - Grade distribution histogram (`S`, `A`, `B`, `C`, `D`, `E`, `F`)
   - Subject-wise performance summary
9. **Generate Merit List**:
   - Algorithm 1: **Custom BST Reverse In-Order Traversal** ($\mathcal{O}(N)$)
   - Algorithm 2: **Custom QuickSort by GPA** (Average $\mathcal{O}(N \log N)$)
10. **Student Alerts**:
    - Evaluates students using a **Custom Max-Heap (Priority Queue)**:
      - Priority 3 (Critical): Debarred attendance ($<65\%$) and Arrears/Failing marks
      - Priority 2 (Warning): Low attendance ($65-74\%$)
      - Priority 1 (Honors): GPA $\ge 9.0$ (Dean's List Candidates)
    - Automatically enqueues at-risk students into a FIFO **Counseling Queue** for sequential counseling actions.
11. **Undo / Redo Changes**: Powered by twin `Stack` instances implementing the Command Pattern. Reverses or re-applies Add, Delete, Update, Mark adjustments, and Attendance updates.
12. **Subject Prerequisite Checker**:
    - Models the University Curriculum as a **Directed Acyclic Graph (DAG)**.
    - Uses DFS to trace all transitive prerequisite chains.
    - Evaluates whether a student satisfies all prerequisites to register for a course.
    - Uses **Kahn's Algorithm (Topological Sort)** to display the optimal course learning sequence.
13. **Save Records**: Persists all records into `data/students.txt`.
14. **Load Records**: Loads and deserializes records, recovering gracefully from missing or damaged lines.
0. **Exit**: Automatically saves state before terminating.

---

## How to Build and Run

### Using GCC / MinGW (C++14 / C++17)

```bash
# Navigate to the workspace directory
cd "c:/VIT/third sem/DSA/student_"

# Compile all modules
g++ -std=c++14 main.cpp Student.cpp Analytics.cpp FileManager.cpp AcademicManager.cpp -o StudentAcademicDashboard.exe

# Run the program
./StudentAcademicDashboard.exe
```

### Using MSVC (Visual Studio Developer Command Prompt)

```cmd
cl /EHsc /std:c++14 main.cpp Student.cpp Analytics.cpp FileManager.cpp AcademicManager.cpp /Fe:StudentAcademicDashboard.exe
StudentAcademicDashboard.exe
```

---

## Verification & Restart Survival Test

The application has been tested and verified:
1. **Compilation**: Clean compilation with 0 warnings and 0 errors.
2. **Initial Seeding**: Pre-loaded with realistic sample student records across departments (CSE, ECE, MECH, IT) and semesters.
3. **Restart Survival**: Modifying or creating records and exiting safely writes them to `data/students.txt`. Relaunching the program automatically loads and verifies the saved records.
