//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_DEUTILS_H
#define SPA_DEUTILS_H
#include <string>
#include <vector>
#include <list>
#include "AST/AssignNode.h"

using namespace std;
namespace TestDE {

    class DEUtils {
    public:
        static bool containsSameElementPair(list<vector<string>> lst, vector<vector<string>> vec);
        static bool isSamePattern(vector<pair<pair<int, string>, shared_ptr<AssignNode>>> actual,
                                    vector<pair<pair<int, string>, shared_ptr<AssignNode>>> expected);
    };

} // TestDE

#endif //SPA_DEUTILS_H
