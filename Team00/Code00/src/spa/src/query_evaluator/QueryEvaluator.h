//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYEVALUATOR_H
#define SPA_QUERYEVALUATOR_H

#include "QueryResult.h"
#include "query_builder/commons/Query.h"
#include "pkb/DataRetriever.h"
#include "DataPreprocessor.h"


using namespace QB;

namespace QE {

    class QueryEvaluator {
        shared_ptr<DataPreprocessor> dataPreprocessor;
        DesignEntity getDesignEntity(Synonym synonym, Query* query);


    public:
        explicit QueryEvaluator(shared_ptr<DataPreprocessor> dataPreprocessor);
        QueryResult evaluate(Query* query);
        QueryResult evaluateNoConditionQuery(Query* query);

    };

} // QE

#endif //SPA_QUERYEVALUATOR_H
