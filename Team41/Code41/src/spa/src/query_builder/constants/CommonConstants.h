//
// Created by Xingchen Lin on 8/10/22.
//

#ifndef SPA_COMMONCONSTANTS_H
#define SPA_COMMONCONSTANTS_H

#include <string>

using namespace std;

namespace QB {

class CommonConstants {
 public:
  static const string DOT;

  static const int ZERO;
  static const int ONE;
  static const int TWO;
  static const int THREE;

  static const string PQL_INVALID_ATTR_REF;
  static const string PQL_INVALID_DESIGN_ENTITY;
  static const string PQL_INVALID_IDENT;
  static const string PQL_INVALID_REF_TYPE;
  static const string PQL_INVALID_RELATION_TYPE;
  static const string PQL_INVALID_SYNONYM;
  static const string PQL_INVALID_WITH_REF_TYPE;
};

}  // namespace QB

#endif  // SPA_COMMONCONSTANTS_H
