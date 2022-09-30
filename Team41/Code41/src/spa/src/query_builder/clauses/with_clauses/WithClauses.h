//
// Created by Xingchen Lin on 20/9/22.
//

#ifndef SPA_WITHCLAUSES_H
#define SPA_WITHCLAUSES_H

#include "query_builder/commons/Ident.h"
#include "query_builder/commons/WithRef.h"
#include "query_builder/commons/WithRef.h"
#include "pkb/table/Table.h"
#include "query_evaluator/IVisitor.h"
#include "query_builder/clauses/AbstractClause.h"

using namespace QB;

namespace QB {
    class WithClause : public AbstractClause, public enable_shared_from_this<WithClause> {
    public:
        WithRef lhs;
        WithRef rhs;

        WithRefType lhsType() const;
        WithRefType rhsType() const;
        static WithRefType getWithRefType(int idx);
        static const int WITHREF_IDENT_IDX = 0;
        static const int WITHREF_INT_IDX = 1;
        static const int WITHREF_ATTR_REF_IDX = 2;

        WithClause(WithRef lhs, WithRef rhs);

        bool operator==(const WithClause& clause) const;
        ostream& print(ostream& os) const;
        Table accept(shared_ptr<IVisitor> visitor) override;
    };

} // QB

#endif //SPA_WITHCLAUSES_H
