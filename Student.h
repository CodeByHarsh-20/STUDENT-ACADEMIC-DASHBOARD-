#ifndef STUDENT_H
#define STUDENT_H

#include <bits/stdc++.h>
#include "Person.h"
using namespace std;

class Student : public Person {
private:
    string studentId;
    string department;
    int semester;
    map<string, double> subjectMarks; // subject name -> mark [0.0, 100.0]
    int totalClasses;
    int attendedClasses;
    vector<string> completedCourses; // course codes completed by student

public:
    Student();
    Student(const string& id, const string& name, int age, const string& email,
            const string& dept, int sem);

    // Getters and Setters
    string getId() const;
    void setId(const string& id);

    string getDepartment() const;
    void setDepartment(const string& dept);

    int getSemester() const;
    void setSemester(int sem);

    int getTotalClasses() const;
    int getAttendedClasses() const;
    void setAttendance(int attended, int total);

    const map<string, double>& getMarks() const;
    void setMark(const string& subject, double score);
    bool removeMark(const string& subject);

    const vector<string>& getCompletedCourses() const;
    void addCompletedCourse(const string& courseCode);
    bool hasCompletedCourse(const string& courseCode) const;

    // Academic Calculations
    double calculatePercentage() const;
    double calculateGPA() const;
    string calculateGrade() const;
    double getAttendancePercentage() const;
    bool isAttendanceEligible() const;
    string getAttendanceStatus() const;
    bool hasFailingSubjects() const;
    vector<string> getFailedSubjects() const;

    // Polymorphic display
    void displaySummary() const override;
    void displayDetailedReport() const;

    // Operators
    bool operator<(const Student& other) const;
    bool operator>(const Student& other) const;
    bool operator==(const Student& other) const;
    friend ostream& operator<<(ostream& os, const Student& s);

    // Serialization format: ID|Name|Age|Email|Dept|Sem|Attended|Total|Sub1:Mark1,Sub2:Mark2|Course1,Course2
    string serialize() const;
    static Student deserialize(const string& line);
};

#endif // STUDENT_H
