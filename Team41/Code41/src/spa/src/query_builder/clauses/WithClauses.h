//
// Created by Xingchen Lin on 20/9/22.
//

#ifndef SPA_WITHCLAUSES_H
#define SPA_WITHCLAUSES_H

#include "query_builder/commons/Ident.h"
#include "query_builder/commons/AttrRef.h"

namespace QB {

    enum WithRefType {IDENT, INT, ATTR_REF};
    using WithRef = variant<Ident, int, AttrRef>;

    class WithClause {
    public:
        WithRef lhs;
        WithRef rhs;

        WithClause(WithRef lhs, WithRef rhs);

        WithRefType lhsType();
        WithRefType rhsType();
        static WithRefType getWithRefType(int idx);
        static const int WITHREF_IDENT_IDX = 0;
        static const int WITHREF_INT_IDX = 1;
        static const int WITHREF_ATTR_REF_IDX = 2;


        bool isSameWithRefType();

        bool operator==(const WithClause& withClauses) const {
            return lhs == withClauses.lhs && rhs == withClauses.rhs;
        }

        // For printing
        std::ostream & print(std::ostream & os) const {
            // Print the derived class specific information.
            os << "With clause";
            return os;
        }
    };

} // QB

#endif //SPA_WITHCLAUSES_H
