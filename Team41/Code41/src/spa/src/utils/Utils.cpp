//
// Created by Xingchen Lin on 9/9/22.
//

#include "Utils.h"
#include <unordered_set>

const std::regex NAME_REGEX = std::regex("[a-zA-Z](\\d|[a-zA-Z])*");

bool Utils::isValidName(string str) {
    return regex_match(str, NAME_REGEX);
}

bool Utils::isValidNumber(string str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

unordered_set<string> Utils::getVariablesFromExprString(string expr) {
    unordered_set<string> ans;
    unordered_set<char> ignored = {'*', '/', '%', '+', '-', '(', ')', ' ', '=', '<', '>', '!', '|', '&'};
    string acc = "";
    for (int i = 0 ; i < expr.length(); i++) {
        if (ignored.find(expr[i]) == ignored.end()) { // char not in ignored
            acc += expr[i];
        } else {
            if (!acc.empty() && !isValidNumber(acc)) {
                ans.insert(acc);
            }
            acc = "";
        }
    }
    if (!acc.empty() && !isValidNumber(acc)) {
        ans.insert(acc);
    }
    return ans;
}