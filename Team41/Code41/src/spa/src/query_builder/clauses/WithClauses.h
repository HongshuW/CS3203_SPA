//
// Created by Xingchen Lin on 20/9/22.
//

#ifndef SPA_WITHCLAUSES_H
#define SPA_WITHCLAUSES_H

#include "query_builder/commons/Ident.h"
#include "query_builder/commons/AttrRef.h"

namespace QB {
    using WithRef = variant<Ident, int, AttrRef>;

    class WithClauses {
    public:
        WithRef lhs;
        WithRef rhs;

        WithClauses(WithRef lhs, WithRef rhs);

        bool operator==(const WithClauses& withClauses) const {
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
