#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <bits/stdc++.h>
#include "Student.h"
using namespace std;

class FileManager {
private:
    string filePath;

public:
    explicit FileManager(const string& path = "data/students.txt");

    void setFilePath(const string& path);
    string getFilePath() const;

    // Core Persistence Methods
    bool saveStudents(const vector<Student>& students);
    vector<Student> loadStudents();

    // Verification and Diagnostics
    bool fileExists() const;
    size_t getFileSize() const;

    // Default Seed Data Generator (for instant evaluation/testing)
    static vector<Student> generateSampleStudents();
};

#endif // FILEMANAGER_H
