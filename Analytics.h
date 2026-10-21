#ifndef ANALYTICS_H
#define ANALYTICS_H

#include <bits/stdc++.h>
#include "Student.h"
#include "dsa/Heap.h"
using namespace std;

struct ClassStatistics {
    size_t totalStudents;
    double averagePercentage;
    double averageGPA;
    double highestGPA;
    double lowestGPA;
    double standardDeviation;
    double passPercentage;
    size_t eligibleAttendanceCount;
    size_t warningAttendanceCount;
    size_t debarredAttendanceCount;
};

class Analytics {
public:
    // Core Statistics
    static ClassStatistics computeClassStatistics(const vector<Student>& students);

    // Grade Distribution Histogram
    static map<string, int> computeGradeDistribution(const vector<Student>& students);

    // Subject-wise Breakdown
    static void displaySubjectWiseBreakdown(const vector<Student>& students);

    // Top-K students using custom Heap (O(N log K))
    static vector<Student> getTopKStudents(const vector<Student>& students, size_t k);

    // Custom Sorting Algorithms for Merit List
    static void quickSortByGPA(vector<Student>& students, int low, int high);
    static void mergeSortByGPA(vector<Student>& students, int left, int right);

    // Binary Search on Sorted Student Vector
    static int binarySearchById(const vector<Student>& sortedStudents, const string& targetId);

    // Display formatted reports
    static void printAnalyticsReport(const vector<Student>& students);

private:
    static int partition(vector<Student>& students, int low, int high);
    static void merge(vector<Student>& students, int left, int mid, int right);
};

#endif // ANALYTICS_H
