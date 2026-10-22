#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Custom Hash Table with Separate Chaining Collision Resolution
 * Automatically rehashes when load factor exceeds 0.75.
 * 
 * Time Complexity Analysis:
 * - Insert:  Average O(1), Worst O(N) during rehash or extreme collisions
 * - Lookup:  Average O(1), Worst O(N)
 * - Delete:  Average O(1), Worst O(N)
 * Space Complexity: O(M + N) where M is bucket capacity and N is element count
 */
template <typename K, typename V>
class HashTable {
private:
    struct HashEntry {
        K key;
        V value;
        HashEntry* next;
        HashEntry(const K& k, const V& v) : key(k), value(v), next(nullptr) {}
    };

    vector<HashEntry*> table;
    size_t capacity;
    size_t numElements;
    const float MAX_LOAD_FACTOR = 0.75f;

    size_t hashFunction(const K& key) const {
        // Generic hash function using std::hash
        return hash<K>{}(key) % capacity;
    }

    void rehash() {
        size_t oldCapacity = capacity;
        vector<HashEntry*> oldTable = table;

        capacity = capacity * 2 + 1; // Grow to next prime-like capacity
        table.assign(capacity, nullptr);
        numElements = 0;

        for (size_t i = 0; i < oldCapacity; ++i) {
            HashEntry* curr = oldTable[i];
            while (curr) {
                insert(curr->key, curr->value);
                HashEntry* temp = curr;
                curr = curr->next;
                delete temp;
            }
        }
    }

public:
    explicit HashTable(size_t initialCapacity = 31)
        : capacity(initialCapacity), numElements(0) {
        table.assign(capacity, nullptr);
    }

    ~HashTable() {
        clear();
    }

    // Copy constructor
    HashTable(const HashTable& other) : capacity(other.capacity), numElements(0) {
        table.assign(capacity, nullptr);
        for (size_t i = 0; i < other.capacity; ++i) {
            HashEntry* curr = other.table[i];
            while (curr) {
                insert(curr->key, curr->value);
                curr = curr->next;
            }
        }
    }

    // Copy assignment
    HashTable& operator=(const HashTable& other) {
        if (this != &other) {
            clear();
            capacity = other.capacity;
            table.assign(capacity, nullptr);
            for (size_t i = 0; i < other.capacity; ++i) {
                HashEntry* curr = other.table[i];
                while (curr) {
                    insert(curr->key, curr->value);
                    curr = curr->next;
                }
            }
        }
        return *this;
    }

    bool insert(const K& key, const V& value) {
        if ((float)(numElements + 1) / capacity > MAX_LOAD_FACTOR) {
            rehash();
        }

        size_t bucketIndex = hashFunction(key);
        HashEntry* curr = table[bucketIndex];

        // Check if key already exists, update value if so
        while (curr) {
            if (curr->key == key) {
                curr->value = value;
                return false; // updated existing
            }
            curr = curr->next;
        }

        // Insert new entry at the head of bucket
        HashEntry* newEntry = new HashEntry(key, value);
        newEntry->next = table[bucketIndex];
        table[bucketIndex] = newEntry;
        numElements++;
        return true; // inserted fresh
    }

    V* find(const K& key) {
        size_t bucketIndex = hashFunction(key);
        HashEntry* curr = table[bucketIndex];
        while (curr) {
            if (curr->key == key) {
                return &(curr->value);
            }
            curr = curr->next;
        }
        return nullptr;
    }

    const V* find(const K& key) const {
        size_t bucketIndex = hashFunction(key);
        HashEntry* curr = table[bucketIndex];
        while (curr) {
            if (curr->key == key) {
                return &(curr->value);
            }
            curr = curr->next;
        }
        return nullptr;
    }

    bool contains(const K& key) const {
        return find(key) != nullptr;
    }

    bool remove(const K& key) {
        size_t bucketIndex = hashFunction(key);
        HashEntry* curr = table[bucketIndex];
        HashEntry* prev = nullptr;

        while (curr) {
            if (curr->key == key) {
                if (prev == nullptr) {
                    table[bucketIndex] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                delete curr;
                numElements--;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }
        return false;
    }

    vector<K> getKeys() const {
        vector<K> keys;
        keys.reserve(numElements);
        for (size_t i = 0; i < capacity; ++i) {
            HashEntry* curr = table[i];
            while (curr) {
                keys.push_back(curr->key);
                curr = curr->next;
            }
        }
        return keys;
    }

    vector<V> getValues() const {
        vector<V> values;
        values.reserve(numElements);
        for (size_t i = 0; i < capacity; ++i) {
            HashEntry* curr = table[i];
            while (curr) {
                values.push_back(curr->value);
                curr = curr->next;
            }
        }
        return values;
    }

    void clear() {
        for (size_t i = 0; i < capacity; ++i) {
            HashEntry* curr = table[i];
            while (curr) {
                HashEntry* temp = curr;
                curr = curr->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        numElements = 0;
    }

    size_t size() const { return numElements; }
    bool empty() const { return numElements == 0; }
    size_t getCapacity() const { return capacity; }
    float getLoadFactor() const { return (float)numElements / capacity; }
};

#endif // HASHTABLE_H
