#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Singly Linked List Implementation
 * 
 * Time Complexity Analysis:
 * - push_front: O(1)
 * - push_back:  O(1) (with tail pointer)
 * - pop_front:  O(1)
 * - remove:     O(N) search and unlink
 * - find:       O(N) linear traversal
 * - clear:      O(N) deallocates all nodes
 * Space Complexity: O(N) where N is number of elements
 */
template <typename T>
class LinkedList {
private:
    struct Node {
        T data;
        Node* next;
        explicit Node(const T& val) : data(val), next(nullptr) {}
    };

    Node* head;
    Node* tail;
    size_t count;

public:
    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~LinkedList() {
        clear();
    }

    // Copy Constructor (Deep Copy)
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* curr = other.head;
        while (curr) {
            push_back(curr->data);
            curr = curr->next;
        }
    }

    // Copy Assignment Operator
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* curr = other.head;
            while (curr) {
                push_back(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        count++;
    }

    void push_front(const T& val) {
        Node* newNode = new Node(val);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head = newNode;
        }
        count++;
    }

    bool pop_front(T& outVal) {
        if (empty()) return false;
        Node* temp = head;
        outVal = head->data;
        head = head->next;
        if (!head) tail = nullptr;
        delete temp;
        count--;
        return true;
    }

    bool remove(function<bool(const T&)> predicate) {
        if (empty()) return false;

        Node* curr = head;
        Node* prev = nullptr;

        while (curr) {
            if (predicate(curr->data)) {
                if (prev == nullptr) {
                    head = curr->next;
                    if (!head) tail = nullptr;
                } else {
                    prev->next = curr->next;
                    if (curr == tail) tail = prev;
                }
                delete curr;
                count--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    T* find(function<bool(const T&)> predicate) {
        Node* curr = head;
        while (curr) {
            if (predicate(curr->data)) {
                return &(curr->data);
            }
            curr = curr->next;
        }
        return nullptr;
    }

    const T* find(function<bool(const T&)> predicate) const {
        Node* curr = head;
        while (curr) {
            if (predicate(curr->data)) {
                return &(curr->data);
            }
            curr = curr->next;
        }
        return nullptr;
    }

    void forEach(function<void(T&)> func) {
        Node* curr = head;
        while (curr) {
            func(curr->data);
            curr = curr->next;
        }
    }

    void forEach(function<void(const T&)> func) const {
        Node* curr = head;
        while (curr) {
            func(curr->data);
            curr = curr->next;
        }
    }

    vector<T> toVector() const {
        vector<T> vec;
        vec.reserve(count);
        Node* curr = head;
        while (curr) {
            vec.push_back(curr->data);
            curr = curr->next;
        }
        return vec;
    }

    void clear() {
        Node* curr = head;
        while (curr) {
            Node* nextNode = curr->next;
            delete curr;
            curr = nextNode;
        }
        head = tail = nullptr;
        count = 0;
    }

    size_t size() const { return count; }
    bool empty() const { return count == 0; }
};

#endif // LINKEDLIST_H
