#ifndef QUEUE_H
#define QUEUE_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Custom Generic Queue Implementation (FIFO - First In First Out)
 * Using linked node architecture to guarantee strictly O(1) enqueue and dequeue.
 * 
 * Time Complexity Analysis:
 * - enqueue: O(1)
 * - dequeue: O(1)
 * - front:   O(1)
 * - empty:   O(1)
 * - size:    O(1)
 * Space Complexity: O(N)
 */
template <typename T>
class Queue {
private:
    struct QueueNode {
        T data;
        QueueNode* next;
        explicit QueueNode(const T& val) : data(val), next(nullptr) {}
    };

    QueueNode* frontNode;
    QueueNode* rearNode;
    size_t count;

public:
    Queue() : frontNode(nullptr), rearNode(nullptr), count(0) {}

    ~Queue() {
        clear();
    }

    Queue(const Queue& other) : frontNode(nullptr), rearNode(nullptr), count(0) {
        QueueNode* curr = other.frontNode;
        while (curr) {
            enqueue(curr->data);
            curr = curr->next;
        }
    }

    Queue& operator=(const Queue& other) {
        if (this != &other) {
            clear();
            QueueNode* curr = other.frontNode;
            while (curr) {
                enqueue(curr->data);
                curr = curr->next;
            }
        }
        return *this;
    }

    void enqueue(const T& item) {
        QueueNode* newNode = new QueueNode(item);
        if (empty()) {
            frontNode = rearNode = newNode;
        } else {
            rearNode->next = newNode;
            rearNode = newNode;
        }
        count++;
    }

    void dequeue() {
        if (empty()) {
            throw DataStructureException("Queue underflow: Cannot dequeue from an empty queue.");
        }
        QueueNode* temp = frontNode;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
        count--;
    }

    T dequeueValue() {
        if (empty()) {
            throw DataStructureException("Queue underflow: Cannot dequeue from an empty queue.");
        }
        QueueNode* temp = frontNode;
        T val = frontNode->data;
        frontNode = frontNode->next;
        if (!frontNode) rearNode = nullptr;
        delete temp;
        count--;
        return val;
    }

    T& front() {
        if (empty()) {
            throw DataStructureException("Queue empty: Cannot access front element.");
        }
        return frontNode->data;
    }

    const T& front() const {
        if (empty()) {
            throw DataStructureException("Queue empty: Cannot access front element.");
        }
        return frontNode->data;
    }

    bool empty() const {
        return count == 0;
    }

    size_t size() const {
        return count;
    }

    void clear() {
        while (!empty()) {
            dequeue();
        }
    }
};

#endif // QUEUE_H
