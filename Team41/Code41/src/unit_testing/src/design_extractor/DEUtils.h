//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_DEUTILS_H
#define SPA_DEUTILS_H
#include <list>
#include <string>
#include <vector>

#include "AST/AssignNode.h"

using namespace std;
namespace TestDE {

class DEUtils {
 public:
  static bool vectorStringContainsSameElements(vector<string> actual,
                                               vector<string> expected);
  static bool stringCmp(string s1, string s2);
  static bool containsSameElementPair(list<vector<string>> lst,
                                      vector<vector<string>> vec);
  static bool containsSameElementTuple(list<vector<string>> lst,
                                       vector<vector<string>> vec);
  static bool isSamePattern(
      vector<pair<pair<int, string>, shared_ptr<AssignNode>>> actual,
      vector<pair<pair<int, string>, shared_ptr<AssignNode>>> expected);
};

}  // namespace TestDE

#endif  // SPA_DEUTILS_H
