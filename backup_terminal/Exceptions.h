#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <bits/stdc++.h>
using namespace std;

// Custom Exception Hierarchy demonstrating C++ Inheritance and Exception Handling
class AcademicException : public runtime_error {
public:
    explicit AcademicException(const string& message)
        : runtime_error(message) {}
};

class StudentNotFoundException : public AcademicException {
public:
    explicit StudentNotFoundException(const string& studentId)
        : AcademicException("Error: Student with ID '" + studentId + "' was not found.") {}
};

class DuplicateStudentException : public AcademicException {
public:
    explicit DuplicateStudentException(const string& studentId)
        : AcademicException("Error: Student with ID '" + studentId + "' already exists.") {}
};

class InvalidInputException : public AcademicException {
public:
    explicit InvalidInputException(const string& message)
        : AcademicException("Validation Error: " + message) {}
};

class DataStructureException : public AcademicException {
public:
    explicit DataStructureException(const string& message)
        : AcademicException("Data Structure Error: " + message) {}
};

class FileIOException : public AcademicException {
public:
    explicit FileIOException(const string& message)
        : AcademicException("File I/O Error: " + message) {}
};

#endif // EXCEPTIONS_H
