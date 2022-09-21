//
// Created by Nafour on 20/9/22.
//

#ifndef SPA_QUERYTUPLERESULT_H
#define SPA_QUERYTUPLERESULT_H
#include "QueryResult.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/commons/Declaration.h"


namespace QE {

    class QueryTupleResult : public QueryResult {
    public:
        shared_ptr<vector<Elem>> returnTuple;
        shared_ptr<vector<Declaration>> declarations;
        explicit QueryTupleResult(shared_ptr<vector<Elem>> returnTuple, shared_ptr<vector<Declaration>> declarations);

    };

} // QE

#endif //SPA_QUERYTUPLERESULT_H
