//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYEVALUATOR_H
#define SPA_QUERYEVALUATOR_H

#include "QueryResult.h"
#include "query_builder/commons/Query.h"
#include "pkb/DataRetriever.h"
#include "DataPreprocessor.h"
#include "ClauseVisitor.h"

using namespace QB;

namespace QE {

    class QueryEvaluator {
        shared_ptr<DataRetriever> dataRetriever;
        Declarations declarations;

        DesignEntity getDesignEntity(Synonym synonym);

        vector<string> evaluateNoConditionQuery(shared_ptr<Query> query, shared_ptr<ClauseVisitor> clauseVisitor);

        std::string join(std::vector<std::string> const &strings, std::string delim);

        vector<string> removeDup(vector<string> vec);

        vector<string>
        formatConditionalQueryResult(Table resultTable, shared_ptr<vector<Elem>> tuple, shared_ptr<Query> query,
                                     shared_ptr<DataPreprocessor> dataPreprocessor);

        vector<string> getAttributeValuesOfCol(Table resultTable, AttrRef attrRef);

        vector<string> projectResult(Table resultTable, shared_ptr<vector<Elem>> tuple);


    public:
        explicit QueryEvaluator(shared_ptr<DataRetriever> dataRetriever);

        vector<string> evaluate(shared_ptr<Query> query);
    };

} // QE

#endif //SPA_QUERYEVALUATOR_H
