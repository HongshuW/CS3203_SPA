//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYEVALUATOR_H
#define SPA_QUERYEVALUATOR_H

#include "QueryResult.h"
#include "query_builder/commons/Query.h"
#include "pkb/DataRetriever.h"
#include "DataPreprocessor.h"
#include "ConcreteClauseVisitor.h"

using namespace QB;

namespace QE {

    class QueryEvaluator {
        shared_ptr<DataRetriever> dataRetriever;
        Declarations declarations;

        DesignEntity getDesignEntity(Synonym synonym);

        vector<string> evaluateNoConditionQuery(shared_ptr<Query> query, shared_ptr<ConcreteClauseVisitor> clauseVisitor);

        std::string join(std::vector<std::string> const &strings, std::string delim);

        vector<string> removeDup(vector<string> vec);

        vector<string>
        formatConditionalQueryResult(Table resultTable, shared_ptr<vector<Elem>> tuple, shared_ptr<Query> query,
                                     shared_ptr<DataPreprocessor> dataPreprocessor);

        vector<string> projectResult(Table resultTable, shared_ptr<vector<Elem>> tuple);

        typedef shared_ptr<unordered_map<string, shared_ptr<unordered_set<int>>>>  ViewedDups;
        int getUnvisitedColIdxByName(const string& colName, ViewedDups viewedDupsMap, const Table& table);

    public:
        explicit QueryEvaluator(shared_ptr<DataRetriever> dataRetriever);

        vector<string> evaluate(shared_ptr<Query> query);
    };

} // QE

#endif //SPA_QUERYEVALUATOR_H
