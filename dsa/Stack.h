#ifndef STACK_H
#define STACK_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Custom Generic Stack Implementation (LIFO - Last In First Out)
 * 
 * Time Complexity Analysis:
 * - push:  O(1) amortized
 * - pop:   O(1)
 * - top:   O(1)
 * - empty: O(1)
 * - size:  O(1)
 * Space Complexity: O(N)
 */
template <typename T>
class Stack {
private:
    vector<T> elements;

public:
    Stack() = default;

    void push(const T& item) {
        elements.push_back(item);
    }

    void pop() {
        if (empty()) {
            throw DataStructureException("Stack underflow: Cannot pop from an empty stack.");
        }
        elements.pop_back();
    }

    T popValue() {
        if (empty()) {
            throw DataStructureException("Stack underflow: Cannot pop from an empty stack.");
        }
        T topItem = elements.back();
        elements.pop_back();
        return topItem;
    }

    T& top() {
        if (empty()) {
            throw DataStructureException("Stack empty: Cannot access top element.");
        }
        return elements.back();
    }

    const T& top() const {
        if (empty()) {
            throw DataStructureException("Stack empty: Cannot access top element.");
        }
        return elements.back();
    }

    bool empty() const {
        return elements.empty();
    }

    size_t size() const {
        return elements.size();
    }

    void clear() {
        elements.clear();
    }
};

#endif // STACK_H
