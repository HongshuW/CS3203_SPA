//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYEVALUATOR_H
#define SPA_QUERYEVALUATOR_H

#include "QueryResult.h"
#include "query_builder/commons/Query.h"
#include "PKB.h"

using namespace QB;

namespace QE {

    class QueryEvaluator {
        PKB* pkb;


    public:
        explicit QueryEvaluator(PKB* pkb);
        QueryResult evaluate(Query* query);
        QueryResult evaluateNoConditionQuery(Query* query);
        DesignEntity getDesignEntity(Synonym synonym, Query* query);

    };

} // QE

#endif //SPA_QUERYEVALUATOR_H
