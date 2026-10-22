#ifndef GRAPH_H
#define GRAPH_H

#include <bits/stdc++.h>
#include "../Exceptions.h"
using namespace std;

/**
 * Directed Graph Implementation for Subject Prerequisite Management
 * 
 * Graph Structure:
 * - Vertices: Course codes / Subject names
 * - Directed Edge (U -> V): U is a prerequisite for V (must take U before V)
 * 
 * Algorithms Demonstrated:
 * 1. Cycle Detection: DFS with recursion stack to verify DAG property (no circular dependencies)
 * 2. Topological Sort: Order of courses to take sequentially
 * 3. Prerequisite Evaluation: DFS/BFS to find all transitive requirements
 * 4. Eligibility Check: Verifies if a student has completed all mandatory prerequisites
 * 
 * Complexity Analysis:
 * - Vertex / Edge insertion: O(1) average
 * - DFS / BFS Traversal: O(V + E)
 * - Cycle Detection: O(V + E)
 * - Topological Sort: O(V + E)
 * Space Complexity: O(V + E) for adjacency list
 */
class CourseGraph {
private:
    // course -> list of courses that depend on this course
    unordered_map<string, vector<string>> forwardAdj;
    // course -> list of courses this course directly requires
    unordered_map<string, vector<string>> prerequisiteAdj;
    // course code -> course name
    unordered_map<string, string> courseDetails;

    bool detectCycleDFS(const string& node,
                        unordered_set<string>& visited,
                        unordered_set<string>& recStack,
                        vector<string>& cyclePath) const {
        visited.insert(node);
        recStack.insert(node);
        cyclePath.push_back(node);

        auto it = forwardAdj.find(node);
        if (it != forwardAdj.end()) {
            for (const auto& neighbor : it->second) {
                if (recStack.find(neighbor) != recStack.end()) {
                    cyclePath.push_back(neighbor);
                    return true; // Cycle detected
                }
                if (visited.find(neighbor) == visited.end()) {
                    if (detectCycleDFS(neighbor, visited, recStack, cyclePath)) {
                        return true;
                    }
                }
            }
        }

        cyclePath.pop_back();
        recStack.erase(node);
        return false;
    }

    void getAllPrereqsDFS(const string& course,
                          unordered_set<string>& allPrereqs) const {
        auto it = prerequisiteAdj.find(course);
        if (it != prerequisiteAdj.end()) {
            for (const auto& req : it->second) {
                if (allPrereqs.find(req) == allPrereqs.end()) {
                    allPrereqs.insert(req);
                    getAllPrereqsDFS(req, allPrereqs);
                }
            }
        }
    }

public:
    CourseGraph() = default;

    void addCourse(const string& code, const string& name) {
        courseDetails[code] = name;
        if (forwardAdj.find(code) == forwardAdj.end()) {
            forwardAdj[code] = {};
        }
        if (prerequisiteAdj.find(code) == prerequisiteAdj.end()) {
            prerequisiteAdj[code] = {};
        }
    }

    void addPrerequisite(const string& prerequisite, const string& course) {
        // prerequisite -> course
        if (courseDetails.find(prerequisite) == courseDetails.end()) {
            courseDetails[prerequisite] = prerequisite;
        }
        if (courseDetails.find(course) == courseDetails.end()) {
            courseDetails[course] = course;
        }

        forwardAdj[prerequisite].push_back(course);
        prerequisiteAdj[course].push_back(prerequisite);
    }

    bool hasCycle() const {
        unordered_set<string> visited;
        unordered_set<string> recStack;
        vector<string> path;

        for (const auto& pair : courseDetails) {
            const string& node = pair.first;
            if (visited.find(node) == visited.end()) {
                if (detectCycleDFS(node, visited, recStack, path)) {
                    return true;
                }
            }
        }
        return false;
    }

    vector<string> getDirectPrerequisites(const string& course) const {
        auto it = prerequisiteAdj.find(course);
        if (it != prerequisiteAdj.end()) {
            return it->second;
        }
        return {};
    }

    vector<string> getAllPrerequisites(const string& course) const {
        unordered_set<string> allReqs;
        getAllPrereqsDFS(course, allReqs);
        return vector<string>(allReqs.begin(), allReqs.end());
    }

    bool checkEligibility(const string& targetCourse,
                          const vector<string>& completedCourses,
                          vector<string>& missingPrereqs) const {
        unordered_set<string> completed(completedCourses.begin(), completedCourses.end());
        vector<string> allReqs = getAllPrerequisites(targetCourse);

        missingPrereqs.clear();
        for (const auto& req : allReqs) {
            if (completed.find(req) == completed.end()) {
                missingPrereqs.push_back(req);
            }
        }
        return missingPrereqs.empty();
    }

    vector<string> getTopologicalOrder() const {
        // Kahn's Algorithm (In-degree calculation)
        unordered_map<string, int> inDegree;
        for (const auto& pair : courseDetails) {
            inDegree[pair.first] = 0;
        }

        for (const auto& pair : forwardAdj) {
            for (const auto& dest : pair.second) {
                inDegree[dest]++;
            }
        }

        queue<string> q;
        for (const auto& pair : inDegree) {
            if (pair.second == 0) {
                q.push(pair.first);
            }
        }

        vector<string> result;
        while (!q.empty()) {
            string curr = q.front();
            q.pop();
            result.push_back(curr);

            auto it = forwardAdj.find(curr);
            if (it != forwardAdj.end()) {
                for (const auto& neighbor : it->second) {
                    inDegree[neighbor]--;
                    if (inDegree[neighbor] == 0) {
                        q.push(neighbor);
                    }
                }
            }
        }

        return result;
    }

    string getCourseName(const string& code) const {
        auto it = courseDetails.find(code);
        return (it != courseDetails.end()) ? it->second : code;
    }

    const unordered_map<string, string>& getCourses() const {
        return courseDetails;
    }

    void displayCurriculum() const {
        cout << "\n================ CURRICULUM PREREQUISITE GRAPH ================\n";
        cout << left << setw(12) << "Code" 
             << setw(32) << "Course Name" 
             << "Prerequisites\n";
        cout << string(72, '-') << "\n";

        for (const auto& pair : courseDetails) {
            const string& code = pair.first;
            string name = pair.second;
            auto it = prerequisiteAdj.find(code);
            string prereqStr = "None (Foundation)";
            if (it != prerequisiteAdj.end() && !it->second.empty()) {
                prereqStr = "";
                for (size_t i = 0; i < it->second.size(); ++i) {
                    prereqStr += it->second[i];
                    if (i + 1 < it->second.size()) prereqStr += ", ";
                }
            }
            cout << left << setw(12) << code 
                 << setw(32) << name 
                 << prereqStr << "\n";
        }
        cout << "===============================================================\n";
    }
};

#endif // GRAPH_H
