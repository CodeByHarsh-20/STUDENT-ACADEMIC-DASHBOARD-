#include <bits/stdc++.h>
#include "Analytics.h"
using namespace std;

ClassStatistics Analytics::computeClassStatistics(const vector<Student>& students) {
    ClassStatistics stats = {0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0};
    if (students.empty()) return stats;

    stats.totalStudents = students.size();
    double sumPct = 0.0;
    double sumGPA = 0.0;
    stats.highestGPA = 0.0;
    stats.lowestGPA = 10.0;
    size_t passedCount = 0;

    for (const auto& s : students) {
        double pct = s.calculatePercentage();
        double gpa = s.calculateGPA();
        sumPct += pct;
        sumGPA += gpa;

        if (gpa > stats.highestGPA) stats.highestGPA = gpa;
        if (gpa < stats.lowestGPA)  stats.lowestGPA = gpa;

        if (pct >= 40.0) passedCount++;

        double attPct = s.getAttendancePercentage();
        if (attPct >= 75.0) stats.eligibleAttendanceCount++;
        else if (attPct >= 65.0) stats.warningAttendanceCount++;
        else stats.debarredAttendanceCount++;
    }

    stats.averagePercentage = sumPct / stats.totalStudents;
    stats.averageGPA = sumGPA / stats.totalStudents;
    stats.passPercentage = ((double)passedCount / stats.totalStudents) * 100.0;

    // Standard Deviation calculation: sqrt( sum( (x - mean)^2 ) / N )
    double varianceSum = 0.0;
    for (const auto& s : students) {
        double diff = s.calculatePercentage() - stats.averagePercentage;
        varianceSum += diff * diff;
    }
    stats.standardDeviation = sqrt(varianceSum / stats.totalStudents);

    return stats;
}

map<string, int> Analytics::computeGradeDistribution(const vector<Student>& students) {
    map<string, int> dist = {
        {"S (>=90)", 0},
        {"A (80-89)", 0},
        {"B (70-79)", 0},
        {"C (60-69)", 0},
        {"D (50-59)", 0},
        {"E (40-49)", 0},
        {"F (<40)", 0}
    };

    for (const auto& s : students) {
        double pct = s.calculatePercentage();
        if (s.getMarks().empty()) continue;
        if (pct >= 90.0) dist["S (>=90)"]++;
        else if (pct >= 80.0) dist["A (80-89)"]++;
        else if (pct >= 70.0) dist["B (70-79)"]++;
        else if (pct >= 60.0) dist["C (60-69)"]++;
        else if (pct >= 50.0) dist["D (50-59)"]++;
        else if (pct >= 40.0) dist["E (40-49)"]++;
        else dist["F (<40)"]++;
    }

    return dist;
}

void Analytics::displaySubjectWiseBreakdown(const vector<Student>& students) {
    if (students.empty()) {
        cout << "No student data available for subject breakdown.\n";
        return;
    }

    // Collect all subjects
    map<string, vector<double>> subjectScores;
    for (const auto& s : students) {
        for (const auto& pair : s.getMarks()) {
            subjectScores[pair.first].push_back(pair.second);
        }
    }

    if (subjectScores.empty()) {
        cout << "No subject marks recorded across students.\n";
        return;
    }

    cout << "\n================== SUBJECT-WISE PERFORMANCE ==================\n";
    cout << left << setw(20) << "Subject" 
         << setw(10) << "Students"
         << setw(10) << "Avg Mark" 
         << setw(10) << "Highest" 
         << setw(10) << "Lowest" 
         << "Pass Rate\n";
    cout << string(68, '-') << "\n";

    for (const auto& pair : subjectScores) {
        const string& subj = pair.first;
        const auto& scores = pair.second;
        double sum = 0.0;
        double highest = scores[0];
        double lowest = scores[0];
        size_t passed = 0;

        for (double sc : scores) {
            sum += sc;
            if (sc > highest) highest = sc;
            if (sc < lowest)  lowest = sc;
            if (sc >= 40.0) passed++;
        }

        double avg = sum / scores.size();
        double passRate = ((double)passed / scores.size()) * 100.0;

        cout << left << setw(20) << subj
             << setw(10) << scores.size()
             << fixed << setprecision(1)
             << setw(10) << avg
             << setw(10) << highest
             << setw(10) << lowest
             << passRate << "%\n";
    }
    cout << "==============================================================\n";
}

// Top-K students using custom Heap
// Time Complexity: O(N log K), Space: O(K)
vector<Student> Analytics::getTopKStudents(const vector<Student>& students, size_t k) {
    if (students.empty() || k == 0) return {};
    if (k > students.size()) k = students.size();

    // Functor for Min-Heap of Students: lowest GPA at root
    // When size exceeds K, we pop the minimum, leaving the largest K elements!
    struct StudentMinGpaComp {
        bool operator()(const Student& a, const Student& b) const {
            return a.calculateGPA() > b.calculateGPA(); // inverted for Min-Heap
        }
    };

    Heap<Student, StudentMinGpaComp> minHeap;

    for (const auto& s : students) {
        minHeap.push(s);
        if (minHeap.size() > k) {
            minHeap.pop();
        }
    }

    vector<Student> topK;
    while (!minHeap.empty()) {
        topK.push_back(minHeap.popValue());
    }
    // Reverse to have highest first
    reverse(topK.begin(), topK.end());
    return topK;
}

// QuickSort descending by GPA
int Analytics::partition(vector<Student>& students, int low, int high) {
    double pivotGPA = students[high].calculateGPA();
    int i = low - 1;

    for (int j = low; j < high; ++j) {
        // Higher GPA comes first
        if (students[j].calculateGPA() >= pivotGPA) {
            i++;
            swap(students[i], students[j]);
        }
    }
    swap(students[i + 1], students[high]);
    return i + 1;
}

void Analytics::quickSortByGPA(vector<Student>& students, int low, int high) {
    if (low < high) {
        int pi = partition(students, low, high);
        quickSortByGPA(students, low, pi - 1);
        quickSortByGPA(students, pi + 1, high);
    }
}

// MergeSort descending by GPA
void Analytics::merge(vector<Student>& students, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Student> L(students.begin() + left, students.begin() + mid + 1);
    vector<Student> R(students.begin() + mid + 1, students.begin() + right + 1);

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i].calculateGPA() >= R[j].calculateGPA()) {
            students[k++] = L[i++];
        } else {
            students[k++] = R[j++];
        }
    }
    while (i < n1) students[k++] = L[i++];
    while (j < n2) students[k++] = R[j++];
}

void Analytics::mergeSortByGPA(vector<Student>& students, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortByGPA(students, left, mid);
        mergeSortByGPA(students, mid + 1, right);
        merge(students, left, mid, right);
    }
}

int Analytics::binarySearchById(const vector<Student>& sortedStudents, const string& targetId) {
    int low = 0;
    int high = (int)sortedStudents.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (sortedStudents[mid].getId() == targetId) {
            return mid;
        }
        if (sortedStudents[mid].getId() < targetId) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1;
}

void Analytics::printAnalyticsReport(const vector<Student>& students) {
    if (students.empty()) {
        cout << "\n[!] No students enrolled to compute analytics.\n";
        return;
    }

    ClassStatistics stats = computeClassStatistics(students);
    auto gradeDist = computeGradeDistribution(students);

    cout << "\n==================================================================\n";
    cout << "                 INSTITUTION PERFORMANCE DASHBOARD                \n";
    cout << "==================================================================\n";
    cout << " Total Students Enrolled : " << stats.totalStudents << "\n";
    cout << " Class Average Percentage: " << fixed << setprecision(2) << stats.averagePercentage << "%\n";
    cout << " Class Average GPA (10)  : " << fixed << setprecision(2) << stats.averageGPA << "\n";
    cout << " Highest GPA Recorded    : " << stats.highestGPA << "\n";
    cout << " Lowest GPA Recorded     : " << stats.lowestGPA << "\n";
    cout << " Standard Deviation (SD) : " << stats.standardDeviation << " pts (variance measure)\n";
    cout << " Overall Pass Percentage : " << stats.passPercentage << "%\n";
    cout << "------------------------------------------------------------------\n";
    cout << " ATTENDANCE COMPLIANCE BREAKDOWN:\n";
    cout << "   - Exam Eligible (>=75%) : " << stats.eligibleAttendanceCount 
         << " (" << (stats.eligibleAttendanceCount * 100 / stats.totalStudents) << "%)\n";
    cout << "   - Medical Warning (65-74%): " << stats.warningAttendanceCount 
         << " (" << (stats.warningAttendanceCount * 100 / stats.totalStudents) << "%)\n";
    cout << "   - Debarred From Exam (<65%): " << stats.debarredAttendanceCount 
         << " (" << (stats.debarredAttendanceCount * 100 / stats.totalStudents) << "%)\n";
    cout << "------------------------------------------------------------------\n";
    cout << " GRADE DISTRIBUTION HISTOGRAM:\n";
    for (const auto& pair : gradeDist) {
        int count = pair.second;
        string bar(count * 2, '#');
        cout << "   " << left << setw(14) << pair.first << " : " 
             << setw(3) << count << " | " << bar << "\n";
    }
    
    displaySubjectWiseBreakdown(students);
}
