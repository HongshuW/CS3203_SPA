//
// Created by Nafour on 11/9/22.
//

#ifndef SPA_UTILS_H
#define SPA_UTILS_H
#include "query_builder/commons/Ref.h"

namespace QE {

    class Utils {
    public:
        static unordered_map<QB::RefType, string> refTypeToColNameMap;
        static string getColNameByRefType(QB::RefType refType);
    };

} // QE

#endif //SPA_UTILS_H
