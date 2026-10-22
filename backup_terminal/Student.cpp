#include <bits/stdc++.h>
#include "Student.h"
#include "Exceptions.h"
using namespace std;

Student::Student()
    : Person("", 0, ""), studentId(""), department(""), semester(1),
      totalClasses(0), attendedClasses(0) {}

Student::Student(const string& id, const string& name, int age, const string& email,
                 const string& dept, int sem)
    : Person(name, age, email), studentId(id), department(dept), semester(sem),
      totalClasses(0), attendedClasses(0) {
    if (sem < 1 || sem > 8) {
        throw InvalidInputException("Semester must be between 1 and 8.");
    }
    if (age <= 0 || age > 100) {
        throw InvalidInputException("Invalid age entered.");
    }
}

string Student::getId() const { return studentId; }
void Student::setId(const string& id) { studentId = id; }

string Student::getDepartment() const { return department; }
void Student::setDepartment(const string& dept) { department = dept; }

int Student::getSemester() const { return semester; }
void Student::setSemester(int sem) {
    if (sem < 1 || sem > 8) {
        throw InvalidInputException("Semester must be between 1 and 8.");
    }
    semester = sem;
}

int Student::getTotalClasses() const { return totalClasses; }
int Student::getAttendedClasses() const { return attendedClasses; }

void Student::setAttendance(int attended, int total) {
    if (total < 0 || attended < 0 || attended > total) {
        throw InvalidInputException("Attended classes cannot exceed total classes or be negative.");
    }
    attendedClasses = attended;
    totalClasses = total;
}

const map<string, double>& Student::getMarks() const {
    return subjectMarks;
}

void Student::setMark(const string& subject, double score) {
    if (score < 0.0 || score > 100.0) {
        throw InvalidInputException("Subject marks must be between 0.0 and 100.0.");
    }
    if (subject.empty()) {
        throw InvalidInputException("Subject name cannot be empty.");
    }
    subjectMarks[subject] = score;
}

bool Student::removeMark(const string& subject) {
    auto it = subjectMarks.find(subject);
    if (it != subjectMarks.end()) {
        subjectMarks.erase(it);
        return true;
    }
    return false;
}

const vector<string>& Student::getCompletedCourses() const {
    return completedCourses;
}

void Student::addCompletedCourse(const string& courseCode) {
    if (!hasCompletedCourse(courseCode)) {
        completedCourses.push_back(courseCode);
    }
}

bool Student::hasCompletedCourse(const string& courseCode) const {
    return find(completedCourses.begin(), completedCourses.end(), courseCode) != completedCourses.end();
}

double Student::calculatePercentage() const {
    if (subjectMarks.empty()) return 0.0;
    double sum = 0.0;
    for (const auto& pair : subjectMarks) {
        sum += pair.second;
    }
    return sum / subjectMarks.size();
}

double Student::calculateGPA() const {
    // 10-point GPA scale standard:
    // >= 90: 10.0, >= 80: 9.0, >= 70: 8.0, >= 60: 7.0, >= 50: 6.0, >= 40: 5.0, <40: 0.0
    if (subjectMarks.empty()) return 0.0;
    double totalPoints = 0.0;
    for (const auto& pair : subjectMarks) {
        double m = pair.second;
        if (m >= 90.0) totalPoints += 10.0;
        else if (m >= 80.0) totalPoints += 9.0;
        else if (m >= 70.0) totalPoints += 8.0;
        else if (m >= 60.0) totalPoints += 7.0;
        else if (m >= 50.0) totalPoints += 6.0;
        else if (m >= 40.0) totalPoints += 5.0;
        else totalPoints += 0.0;
    }
    return totalPoints / subjectMarks.size();
}

string Student::calculateGrade() const {
    double pct = calculatePercentage();
    if (subjectMarks.empty()) return "N/A";
    if (pct >= 90.0) return "S (Outstanding)";
    if (pct >= 80.0) return "A (Excellent)";
    if (pct >= 70.0) return "B (Very Good)";
    if (pct >= 60.0) return "C (Good)";
    if (pct >= 50.0) return "D (Average)";
    if (pct >= 40.0) return "E (Pass)";
    return "F (Fail)";
}

double Student::getAttendancePercentage() const {
    if (totalClasses == 0) return 100.0;
    return ((double)attendedClasses / totalClasses) * 100.0;
}

bool Student::isAttendanceEligible() const {
    return getAttendancePercentage() >= 75.0;
}

string Student::getAttendanceStatus() const {
    if (totalClasses == 0) return "N/A (No classes recorded)";
    double pct = getAttendancePercentage();
    if (pct >= 75.0) return "ELIGIBLE";
    if (pct >= 65.0) return "WARNING (Medical condonation required: " + to_string((int)pct) + "%)";
    return "DEBARRED (<65%: " + to_string((int)pct) + "%)";
}

bool Student::hasFailingSubjects() const {
    for (const auto& pair : subjectMarks) {
        if (pair.second < 40.0) return true;
    }
    return false;
}

vector<string> Student::getFailedSubjects() const {
    vector<string> fails;
    for (const auto& pair : subjectMarks) {
        if (pair.second < 40.0) {
            fails.push_back(pair.first + " (" + to_string((int)pair.second) + ")");
        }
    }
    return fails;
}

void Student::displaySummary() const {
    cout << left 
         << setw(10) << studentId 
         << setw(20) << name 
         << setw(8)  << ("Sem " + to_string(semester))
         << setw(12) << department 
         << fixed << setprecision(1)
         << setw(10) << calculatePercentage() 
         << setw(8)  << calculateGPA() 
         << setw(12) << (to_string((int)getAttendancePercentage()) + "%")
         << isAttendanceEligible();
}

void Student::displayDetailedReport() const {
    cout << "\n==================================================================\n";
    cout << "               STUDENT ACADEMIC TRANSCRIPT REPORT                 \n";
    cout << "==================================================================\n";
    cout << " Student ID    : " << studentId << "\n";
    cout << " Full Name     : " << name << "\n";
    cout << " Age / Email   : " << age << " yrs | " << email << "\n";
    cout << " Department    : " << department << "\n";
    cout << " Semester      : " << semester << "\n";
    cout << " Attendance    : " << attendedClasses << "/" << totalClasses 
         << " (" << fixed << setprecision(1) << getAttendancePercentage() << "%) -> "
         << getAttendanceStatus() << "\n";
    cout << "------------------------------------------------------------------\n";
    cout << " SUBJECT MARKS:\n";
    if (subjectMarks.empty()) {
        cout << "   [No subject marks recorded yet]\n";
    } else {
        cout << "   " << left << setw(25) << "Subject Name" 
             << setw(12) << "Score / 100" 
             << "Status\n";
        cout << "   --------------------------------------------\n";
        for (const auto& pair : subjectMarks) {
            cout << "   " << left << setw(25) << pair.first 
                 << fixed << setprecision(1) << setw(12) << pair.second 
                 << (pair.second >= 40.0 ? "PASS" : "FAIL (Arrear)") << "\n";
        }
    }
    cout << "------------------------------------------------------------------\n";
    cout << " Overall Percentage : " << fixed << setprecision(2) << calculatePercentage() << "%\n";
    cout << " GPA (10.0 scale)   : " << fixed << setprecision(2) << calculateGPA() << "\n";
    cout << " Final Letter Grade : " << calculateGrade() << "\n";
    
    cout << " Completed Courses  : ";
    if (completedCourses.empty()) {
        cout << "None";
    } else {
        for (size_t i = 0; i < completedCourses.size(); ++i) {
            cout << completedCourses[i] << (i + 1 < completedCourses.size() ? ", " : "");
        }
    }
    cout << "\n==================================================================\n";
}

bool Student::operator<(const Student& other) const {
    // Sort descending by GPA / Percentage
    if (calculateGPA() != other.calculateGPA()) {
        return calculateGPA() > other.calculateGPA();
    }
    return studentId < other.studentId;
}

bool Student::operator>(const Student& other) const {
    if (calculateGPA() != other.calculateGPA()) {
        return calculateGPA() < other.calculateGPA();
    }
    return studentId > other.studentId;
}

bool Student::operator==(const Student& other) const {
    return studentId == other.studentId;
}

ostream& operator<<(ostream& os, const Student& s) {
    os << "[" << s.studentId << "] " << s.name << " (" << s.department 
       << ", Sem " << s.semester << ") GPA: " << fixed << setprecision(2) << s.calculateGPA();
    return os;
}

string Student::serialize() const {
    ostringstream ss;
    ss << studentId << "|"
       << name << "|"
       << age << "|"
       << email << "|"
       << department << "|"
       << semester << "|"
       << attendedClasses << "|"
       << totalClasses << "|";

    // Serialize marks: Sub1:85.5;Sub2:92.0
    bool first = true;
    for (const auto& pair : subjectMarks) {
        if (!first) ss << ";";
        ss << pair.first << ":" << pair.second;
        first = false;
    }
    ss << "|";

    // Serialize completed courses: CS101;CS102
    first = true;
    for (const auto& c : completedCourses) {
        if (!first) ss << ";";
        ss << c;
        first = false;
    }
    return ss.str();
}

Student Student::deserialize(const string& line) {
    if (line.empty()) return Student();

    vector<string> tokens;
    string token;
    stringstream ss(line);
    while (getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    if (tokens.size() < 8) {
        throw FileIOException("Corrupt student record: invalid token count in line: " + line);
    }

    string id = tokens[0];
    string name = tokens[1];
    int age = stoi(tokens[2]);
    string email = tokens[3];
    string dept = tokens[4];
    int sem = stoi(tokens[5]);
    int attended = stoi(tokens[6]);
    int total = stoi(tokens[7]);

    Student s(id, name, age, email, dept, sem);
    s.setAttendance(attended, total);

    // Parse marks
    if (tokens.size() > 8 && !tokens[8].empty()) {
        stringstream marksStream(tokens[8]);
        string subMark;
        while (getline(marksStream, subMark, ';')) {
            size_t colonPos = subMark.find(':');
            if (colonPos != string::npos) {
                string sub = subMark.substr(0, colonPos);
                double mark = stod(subMark.substr(colonPos + 1));
                s.setMark(sub, mark);
            }
        }
    }

    // Parse completed courses
    if (tokens.size() > 9 && !tokens[9].empty()) {
        stringstream courseStream(tokens[9]);
        string c;
        while (getline(courseStream, c, ';')) {
            if (!c.empty()) {
                s.addCompletedCourse(c);
            }
        }
    }

    return s;
}
