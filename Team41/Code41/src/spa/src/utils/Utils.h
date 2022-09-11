//
// Created by Xingchen Lin on 9/9/22.
//

#ifndef SPA_UTILSSS_H
#define SPA_UTILSSS_H

#include <string>
#include <regex>
#include <unordered_set>

using namespace std;

class Utils {
public:
    static bool isValidName(string str);
    static bool isValidNumber(string str);
    static unordered_set<string> getVariablesFromExprString(string expr);

};

#endif //SPA_UTILSSS_H
