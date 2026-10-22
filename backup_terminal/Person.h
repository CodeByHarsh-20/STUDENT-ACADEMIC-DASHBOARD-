#ifndef PERSON_H
#define PERSON_H

#include <bits/stdc++.h>
using namespace std;

/**
 * Base Abstract Class demonstrating:
 * - Abstraction & Encapsulation
 * - Polymorphism with virtual methods & virtual destructor
 */
class Person {
protected:
    string name;
    int age;
    string email;

public:
    Person() : name(""), age(0), email("") {}
    Person(const string& n, int a, const string& e)
        : name(n), age(a), email(e) {}

    virtual ~Person() = default;

    // Getters and Setters
    string getName() const { return name; }
    void setName(const string& n) { name = n; }

    int getAge() const { return age; }
    void setAge(int a) { age = a; }

    string getEmail() const { return email; }
    void setEmail(const string& e) { email = e; }

    // Pure virtual method for polymorphic display
    virtual void displaySummary() const = 0;
};

#endif // PERSON_H
