//
// Created by Nafour on 11/9/22.
//

#include "DEUtils.h"
#include "iostream"
using namespace std;
namespace TestDE {
    bool DEUtils::containsSameElementPair(list<vector<string>> lst, vector<vector<string>> vec) {
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
} // TestDE