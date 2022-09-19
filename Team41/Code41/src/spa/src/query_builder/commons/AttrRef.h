//
// Created by Xingchen Lin on 19/9/22.
//

#ifndef SPA_ATTRREF_H
#define SPA_ATTRREF_H

#include <string>
#include <unordered_map>
#include "Synonym.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace std;
using namespace QB;

namespace QB {
    enum class AttrName {
        PROC_NAME, // 'procName'
        VAR_NAME, // 'varName'
        VALUE, // 'value'
        STMT_NUMBER // 'stmt#'
    };

    class AttrRef {
    public:
        Synonym synonym;
        AttrName attrName;
        AttrRef(Synonym synonym, AttrName attrName);

        static AttrName getAttrNameFromStr(string& str);

        bool operator==(const AttrRef& attrRef1) const {
            return synonym == attrRef1.synonym && attrName == attrRef1.attrName;
        }

        // For printing
        friend std::ostream& operator<<(std::ostream& os, AttrRef const& attrRef) {
            os << "AttrRef: ";
            os << attrRef.synonym;
            return os;
        }
    };

} // QB

#endif //SPA_ATTRREF_H
