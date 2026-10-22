#ifndef BST_H
#define BST_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Custom Binary Search Tree (BST) Implementation
 * Used for hierarchical student indexing, O(log N) search, 
 * sorted in-order traversal, and range queries.
 * 
 * Time Complexity Analysis:
 * - Insert:        Average O(log N), Worst O(N) (skewed)
 * - Search:        Average O(log N), Worst O(N)
 * - Delete:        Average O(log N), Worst O(N)
 * - Traversal:     O(N) visits every node exactly once
 * Space Complexity: O(N) total storage, O(H) call stack where H is tree height
 */
template <typename Key, typename Value, typename Comparator = less<Key>>
class BST {
private:
    struct TreeNode {
        Key key;
        Value value;
        TreeNode* left;
        TreeNode* right;

        TreeNode(const Key& k, const Value& v)
            : key(k), value(v), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;
    size_t nodeCount;
    Comparator comp;

    TreeNode* insertRecursive(TreeNode* node, const Key& key, const Value& val) {
        if (!node) {
            nodeCount++;
            return new TreeNode(key, val);
        }
        if (comp(key, node->key)) {
            node->left = insertRecursive(node->left, key, val);
        } else if (comp(node->key, key)) {
            node->right = insertRecursive(node->right, key, val);
        } else {
            // Update value if key matches
            node->value = val;
        }
        return node;
    }

    TreeNode* findRecursive(TreeNode* node, const Key& key) const {
        if (!node) return nullptr;
        if (comp(key, node->key)) {
            return findRecursive(node->left, key);
        } else if (comp(node->key, key)) {
            return findRecursive(node->right, key);
        }
        return node;
    }

    TreeNode* findMin(TreeNode* node) const {
        if (!node) return nullptr;
        while (node->left) {
            node = node->left;
        }
        return node;
    }

    TreeNode* findMax(TreeNode* node) const {
        if (!node) return nullptr;
        while (node->right) {
            node = node->right;
        }
        return node;
    }

    TreeNode* deleteRecursive(TreeNode* node, const Key& key, bool& deleted) {
        if (!node) return nullptr;

        if (comp(key, node->key)) {
            node->left = deleteRecursive(node->left, key, deleted);
        } else if (comp(node->key, key)) {
            node->right = deleteRecursive(node->right, key, deleted);
        } else {
            // Found node to delete
            deleted = true;
            nodeCount--;

            // Case 1: Leaf or one child
            if (!node->left) {
                TreeNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                TreeNode* temp = node->left;
                delete node;
                return temp;
            }

            // Case 2: Two children -> Replace with In-order Successor
            TreeNode* successor = findMin(node->right);
            node->key = successor->key;
            node->value = successor->value;
            // Delete successor from right subtree
            bool dummy = false;
            nodeCount++; // compensate for next call decrementing
            node->right = deleteRecursive(node->right, successor->key, dummy);
        }
        return node;
    }

    void inOrderRecursive(TreeNode* node, function<void(const Key&, const Value&)> visit) const {
        if (!node) return;
        inOrderRecursive(node->left, visit);
        visit(node->key, node->value);
        inOrderRecursive(node->right, visit);
    }

    void reverseInOrderRecursive(TreeNode* node, function<void(const Key&, const Value&)> visit) const {
        if (!node) return;
        reverseInOrderRecursive(node->right, visit);
        visit(node->key, node->value);
        reverseInOrderRecursive(node->left, visit);
    }

    void rangeQueryRecursive(TreeNode* node, const Key& low, const Key& high,
                             vector<Value>& results) const {
        if (!node) return;
        if (comp(low, node->key)) {
            rangeQueryRecursive(node->left, low, high, results);
        }
        if (!comp(node->key, low) && !comp(high, node->key)) {
            results.push_back(node->value);
        }
        if (comp(node->key, high)) {
            rangeQueryRecursive(node->right, low, high, results);
        }
    }

    void destroyTree(TreeNode* node) {
        if (!node) return;
        destroyTree(node->left);
        destroyTree(node->right);
        delete node;
    }

    TreeNode* cloneTree(TreeNode* node) {
        if (!node) return nullptr;
        TreeNode* newNode = new TreeNode(node->key, node->value);
        newNode->left = cloneTree(node->left);
        newNode->right = cloneTree(node->right);
        return newNode;
    }

public:
    BST() : root(nullptr), nodeCount(0) {}

    ~BST() {
        clear();
    }

    BST(const BST& other) : root(nullptr), nodeCount(other.nodeCount), comp(other.comp) {
        root = cloneTree(other.root);
    }

    BST& operator=(const BST& other) {
        if (this != &other) {
            clear();
            root = cloneTree(other.root);
            nodeCount = other.nodeCount;
            comp = other.comp;
        }
        return *this;
    }

    void insert(const Key& key, const Value& val) {
        root = insertRecursive(root, key, val);
    }

    Value* search(const Key& key) {
        TreeNode* found = findRecursive(root, key);
        return found ? &(found->value) : nullptr;
    }

    const Value* search(const Key& key) const {
        TreeNode* found = findRecursive(root, key);
        return found ? &(found->value) : nullptr;
    }

    bool remove(const Key& key) {
        bool deleted = false;
        root = deleteRecursive(root, key, deleted);
        return deleted;
    }

    void inOrderTraversal(function<void(const Key&, const Value&)> visit) const {
        inOrderRecursive(root, visit);
    }

    void reverseInOrderTraversal(function<void(const Key&, const Value&)> visit) const {
        reverseInOrderRecursive(root, visit);
    }

    vector<Value> rangeQuery(const Key& low, const Key& high) const {
        vector<Value> results;
        rangeQueryRecursive(root, low, high, results);
        return results;
    }

    bool getMin(Key& outKey, Value& outVal) const {
        TreeNode* m = findMin(root);
        if (!m) return false;
        outKey = m->key;
        outVal = m->value;
        return true;
    }

    bool getMax(Key& outKey, Value& outVal) const {
        TreeNode* m = findMax(root);
        if (!m) return false;
        outKey = m->key;
        outVal = m->value;
        return true;
    }

    size_t size() const { return nodeCount; }
    bool empty() const { return root == nullptr; }

    void clear() {
        destroyTree(root);
        root = nullptr;
        nodeCount = 0;
    }
};

#endif // BST_H
