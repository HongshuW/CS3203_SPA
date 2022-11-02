//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_QEUTILS_H
#define SPA_QEUTILS_H
#include "pkb/table/Table.h"
#include "query_builder/commons/Ref.h"

namespace QE {

class QEUtils {
 public:
  static unordered_map<QB::RefType, string> refTypeToColNameMap;
  static string getColNameByRefType(QB::RefType refType);
  static shared_ptr<Table> getScalarResponse(bool hasResult);
};

}  // namespace QE

#endif  // SPA_QEUTILS_H
