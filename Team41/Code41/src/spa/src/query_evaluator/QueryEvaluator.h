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
        shared_ptr<DataPreprocessor> dataPreprocessor;
        DesignEntity getDesignEntity(Synonym synonym, shared_ptr<Query> query);
        string getDesignEntityColName(DesignEntity entity);
        vector<string> evaluateNoConditionQuery(shared_ptr<Query> query);
        std::string join(std::vector<std::string> const &strings, std::string delim);
        vector<string> removeDup(vector<string> vec);
        bool doesConditionExist(shared_ptr<Query> query, WithClause withClause);
        std::vector<std::string> intersection(std::vector<std::string> v1,
                                              std::vector<std::string> v2);
        std::vector<int> intersection(std::vector<int> v1,
                                                      std::vector<int> v2);
        vector<string> formatConditionalQueryResult(Table resultTable, shared_ptr<vector<Elem>> tuple, shared_ptr<Query> query);
        vector<string> getAttributeValuesOfCol(Table resultTable, AttrRef attrRef, shared_ptr<Query> query);


    public:
        explicit QueryEvaluator(shared_ptr<DataPreprocessor> dataPreprocessor);
        vector<string> evaluate(shared_ptr<Query> query);



    };

} // QE

#endif //SPA_QUERYEVALUATOR_H
