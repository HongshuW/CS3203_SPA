//
// Created by Nafour on 13/9/22.
//

#include "QETestUtils.h"

#include <unordered_set>

namespace QETest {
bool QETestUtils::containsSameElement(vector<string> vec1,
                                      vector<string> vec2) {
  if (vec1.size() != vec2.size()) return false;
  unordered_set<string> set1(vec1.begin(), vec1.end());
  unordered_set<string> set2(vec2.begin(), vec2.end());
  return set1 == set2;
}
}  // namespace QETest