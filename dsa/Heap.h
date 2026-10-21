#ifndef HEAP_H
#define HEAP_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Custom Binary Max/Min Heap Implementation
 * 
 * Used for:
 * - Top-K student ranking (O(N log K) time)
 * - Priority-based Student Alert Processing (Urgent warnings first)
 * 
 * Time Complexity Analysis:
 * - push (insert): O(log N)
 * - pop (extract): O(log N)
 * - top (peek):    O(1)
 * - buildHeap:     O(N)
 * Space Complexity: O(N)
 */
template <typename T, typename Comparator = less<T>>
class Heap {
private:
    vector<T> heapArray;
    Comparator comp;

    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t leftChild(size_t i) const { return 2 * i + 1; }
    size_t rightChild(size_t i) const { return 2 * i + 2; }

    void siftUp(size_t i) {
        while (i > 0 && comp(heapArray[parent(i)], heapArray[i])) {
            swap(heapArray[i], heapArray[parent(i)]);
            i = parent(i);
        }
    }

    void siftDown(size_t i) {
        size_t maxIdx = i;
        size_t left = leftChild(i);
        size_t right = rightChild(i);

        if (left < heapArray.size() && comp(heapArray[maxIdx], heapArray[left])) {
            maxIdx = left;
        }
        if (right < heapArray.size() && comp(heapArray[maxIdx], heapArray[right])) {
            maxIdx = right;
        }

        if (i != maxIdx) {
            swap(heapArray[i], heapArray[maxIdx]);
            siftDown(maxIdx);
        }
    }

public:
    explicit Heap(Comparator c = Comparator()) : comp(c) {}

    void push(const T& val) {
        heapArray.push_back(val);
        siftUp(heapArray.size() - 1);
    }

    void pop() {
        if (empty()) {
            throw DataStructureException("Heap underflow: Cannot pop from empty heap.");
        }
        heapArray[0] = heapArray.back();
        heapArray.pop_back();
        if (!empty()) {
            siftDown(0);
        }
    }

    T popValue() {
        if (empty()) {
            throw DataStructureException("Heap underflow: Cannot pop from empty heap.");
        }
        T topVal = heapArray[0];
        heapArray[0] = heapArray.back();
        heapArray.pop_back();
        if (!empty()) {
            siftDown(0);
        }
        return topVal;
    }

    const T& top() const {
        if (empty()) {
            throw DataStructureException("Heap empty: Cannot access top element.");
        }
        return heapArray[0];
    }

    bool empty() const {
        return heapArray.empty();
    }

    size_t size() const {
        return heapArray.size();
    }

    void clear() {
        heapArray.clear();
    }
};

#endif // HEAP_H
