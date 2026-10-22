#include <bits/stdc++.h>
#include "FileManager.h"
#include "Exceptions.h"
using namespace std;

FileManager::FileManager(const string& path) : filePath(path) {}

void FileManager::setFilePath(const string& path) {
    filePath = path;
}

string FileManager::getFilePath() const {
    return filePath;
}

bool FileManager::fileExists() const {
    ifstream file(filePath);
    return file.good();
}

size_t FileManager::getFileSize() const {
    ifstream file(filePath, ios::binary | ios::ate);
    if (!file.is_open()) return 0;
    return static_cast<size_t>(file.tellg());
}

bool FileManager::saveStudents(const vector<Student>& students) {
    ofstream outFile(filePath);
    if (!outFile.is_open()) {
        throw FileIOException("Unable to open file for writing at: " + filePath);
    }

    // Write header comment
    outFile << "# University Academic Management System Records\n";
    outFile << "# Format: ID|Name|Age|Email|Dept|Sem|Attended|Total|Sub:Mark,...|Course,...\n";

    for (const auto& s : students) {
        outFile << s.serialize() << "\n";
    }

    outFile.close();
    return true;
}

vector<Student> FileManager::loadStudents() {
    if (!fileExists()) {
        throw FileIOException("Records file does not exist at: " + filePath);
    }

    ifstream inFile(filePath);
    if (!inFile.is_open()) {
        throw FileIOException("Could not open records file for reading: " + filePath);
    }

    vector<Student> loadedList;
    string line;
    size_t lineNum = 0;
    size_t corruptCount = 0;

    while (getline(inFile, line)) {
        lineNum++;
        if (line.empty() || line[0] == '#') continue; // skip comments and blank lines

        try {
            Student s = Student::deserialize(line);
            if (!s.getId().empty()) {
                loadedList.push_back(s);
            }
        } catch (const exception& ex) {
            corruptCount++;
            cerr << "[Warning] Line " << lineNum << " skipped due to formatting error: " 
                 << ex.what() << "\n";
        }
    }

    inFile.close();

    if (corruptCount > 0) {
        cout << "[Note] File loaded with " << corruptCount << " skipped corrupted record(s).\n";
    }

    return loadedList;
}

vector<Student> FileManager::generateSampleStudents() {
    vector<Student> samples;

    // Student 1: High Achiever (Dean's List / S Grade)
    Student s1("24BCE1001", "Aarav Sharma", 19, "aarav.sharma@univ.edu", "CSE", 3);
    s1.setAttendance(48, 50); // 96%
    s1.setMark("Data Structures", 95.0);
    s1.setMark("Digital Logic", 92.0);
    s1.setMark("Discrete Math", 94.0);
    s1.setMark("Database Systems", 91.0);
    s1.addCompletedCourse("CS101");
    s1.addCompletedCourse("CS102");
    samples.push_back(s1);

    // Student 2: Consistent Good Performer
    Student s2("24BCE1002", "Diya Patel", 20, "diya.patel@univ.edu", "CSE", 3);
    s2.setAttendance(44, 50); // 88%
    s2.setMark("Data Structures", 84.0);
    s2.setMark("Digital Logic", 80.0);
    s2.setMark("Discrete Math", 78.0);
    s2.setMark("Database Systems", 86.0);
    s2.addCompletedCourse("CS101");
    samples.push_back(s2);

    // Student 3: Attendance Warning Student (68%)
    Student s3("24BCE1003", "Rohan Verma", 20, "rohan.verma@univ.edu", "ECE", 3);
    s3.setAttendance(34, 50); // 68% -> WARNING
    s3.setMark("Data Structures", 72.0);
    s3.setMark("Digital Logic", 68.0);
    s3.setMark("Signals & Systems", 75.0);
    s3.setMark("Circuits", 70.0);
    s3.addCompletedCourse("CS101");
    samples.push_back(s3);

    // Student 4: Debarred and At-Risk Student (<65% attendance & failing marks)
    Student s4("24BCE1004", "Vikram Singh", 21, "vikram.singh@univ.edu", "MECH", 3);
    s4.setAttendance(28, 50); // 56% -> DEBARRED
    s4.setMark("Thermodynamics", 34.0); // Fail
    s4.setMark("Fluid Mechanics", 38.0); // Fail
    s4.setMark("Kinematics", 52.0);
    s4.setMark("Material Science", 45.0);
    samples.push_back(s4);

    // Student 5: Outstanding Senior Student
    Student s5("23BCE0501", "Ananya Iyer", 21, "ananya.iyer@univ.edu", "CSE", 5);
    s5.setAttendance(46, 50); // 92%
    s5.setMark("Operating Systems", 88.0);
    s5.setMark("Computer Networks", 89.0);
    s5.setMark("Design & Analysis of Algo", 93.0);
    s5.setMark("Software Engineering", 85.0);
    s5.addCompletedCourse("CS101");
    s5.addCompletedCourse("CS102");
    s5.addCompletedCourse("CS201");
    s5.addCompletedCourse("CS202");
    samples.push_back(s5);

    // Student 6: Average Student
    Student s6("24BCE1006", "Kabir Sen", 19, "kabir.sen@univ.edu", "IT", 2);
    s6.setAttendance(40, 50); // 80%
    s6.setMark("Programming in C++", 65.0);
    s6.setMark("Digital Electronics", 62.0);
    s6.setMark("Calculus", 58.0);
    s6.setMark("Physics", 70.0);
    s6.addCompletedCourse("CS101");
    samples.push_back(s6);

    return samples;
}
