//
// Created by Xingchen Lin on 19/9/22.
//

#ifndef SPA_ATTRREF_H
#define SPA_ATTRREF_H

#include <string>
#include <unordered_map>

#include "Synonym.h"
#include "query_builder/constants/CommonConstants.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace std;
using namespace QB;

namespace QB {
enum class AttrName {
  PROC_NAME,   // 'procName'
  VAR_NAME,    // 'varName'
  VALUE,       // 'value'
  STMT_NUMBER  // 'stmt#'
};

enum class WithComparingType {
  NAME,
  INTEGER,
};

class AttrRef {
 public:
  Synonym synonym;
  AttrName attrName;
  AttrRef(Synonym synonym, AttrName attrName);

  static AttrName getAttrNameFromStr(string& str);

  [[nodiscard]] string getStrOfAttrName() const;
  [[nodiscard]] string toString() const;
  static string getStrFromAttrName(AttrName& attrName);
  static WithComparingType getWithComparingTypeFromAttrName(AttrName& attrName);

  bool operator==(const AttrRef& attrRef1) const;
};

}  // namespace QB

#endif  // SPA_ATTRREF_H
