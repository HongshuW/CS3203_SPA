//
// Created by Nafour on 11/9/22.
//

#include "DEUtils.h"
#include "iostream"
using namespace std;
namespace TestDE {
    bool DEUtils::containsSameElementPair(list<vector<string>> lst, vector<vector<string>> vec) {
        if (lst.size() != vec.size()) return false;
        std::list<vector<string>>::iterator it;
        for (it = lst.begin(); it != lst.end(); ++it){
            bool found = false;
            for (auto expectedPair: vec) {
                if (it->at(0) == expectedPair[0] && (it->at(1) == expectedPair[1])) {
                    found = true;
                }
            }


            if (!found) {
                cout << it->at(0) + " " + it->at(1) << " not found" << endl;
                return false;
            }
        }
        return true;
    }

    bool DEUtils::containsSameElementTuple(list<vector<string>> lst, vector<vector<string>> vec)
    {
        if (lst.size() != vec.size()) return false;
        std::list<vector<string>>::iterator it;
        for (it = lst.begin(); it != lst.end(); ++it) {
            bool found = false;
            for (auto expectedPair : vec) {
                if (it->at(0) == expectedPair[0] && (it->at(1) == expectedPair[1]) && (it->at(2) == expectedPair[2])) {
                    found = true;
                }
            }


            if (!found) {
                cout << it->at(0) + " " + it->at(1) << " not found" << endl;
                return false;
            }
        }
        return true;
    }

   

    bool DEUtils::isSamePattern(vector<pair<pair<int, string>, shared_ptr<AssignNode>>> actual,
                                vector<pair<pair<int, string>, shared_ptr<AssignNode>>> expected) {
        if (actual.size() != expected.size()) return false;
        for (auto rowActual: actual) {
            bool isFound = false;
            for (auto rowExpected: expected) {
                bool sameLine = rowActual.first.first == rowExpected.first.first;
                bool sameVar = rowActual.first.second == rowExpected.first.second;
                bool sameNode = rowActual.second == rowExpected.second;
                if (sameLine && sameVar && sameNode) {
                    isFound = true;
                }
            }
            if (!isFound) return false;
        }
        return true;
    }
} // TestDE