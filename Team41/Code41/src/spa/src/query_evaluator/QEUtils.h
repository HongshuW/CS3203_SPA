//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_QEUTILS_H
#define SPA_QEUTILS_H
#include "query_builder/commons/Ref.h"
#include "pkb/table/Table.h"

namespace QE {

    class QEUtils {
    public:
        static unordered_map<QB::RefType, string> refTypeToColNameMap;
        static string getColNameByRefType(QB::RefType refType);
        static Table getScalarResponse(bool hasResult);
    };

} // QE

#endif //SPA_QEUTILS_H
