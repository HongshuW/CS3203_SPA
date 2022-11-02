//
// Created by Nafour on 31/8/22.
//

#ifndef SPA_QUERYEVALUATOR_H
#define SPA_QUERYEVALUATOR_H

#include "ConcreteClauseVisitor.h"
#include "DataPreprocessor.h"
#include "QueryOptimizer.h"
#include "pkb/DataRetriever.h"
#include "query_builder/commons/Query.h"

using namespace QB;

namespace QE {

class QueryEvaluator {
  shared_ptr<DataRetriever> dataRetriever;
  Declarations declarations;
  shared_ptr<Query> query;
  vector<string> selected;

  DesignEntity getDesignEntity(Synonym synonym);

  vector<string> evaluateNoConditionSelectTupleQuery(
      shared_ptr<Query> query, shared_ptr<ConcreteClauseVisitor> clauseVisitor);

  vector<string> evaluateSelectBoolQuery(
      shared_ptr<ConcreteClauseVisitor> clauseVisitor,
      shared_ptr<DataPreprocessor> dataPreprocessor, ConnectedClauseGroups ccg);

  vector<string> evaluateSelectTupleQuery(
      shared_ptr<ConcreteClauseVisitor> clauseVisitor,
      shared_ptr<DataPreprocessor> dataPreprocessor, ConnectedClauseGroups ccg);

  vector<string> removeDup(vector<string> vec);

  vector<string> formatConditionalQueryResult(
      shared_ptr<Table> resultTable, shared_ptr<vector<Elem>> tuple,
      shared_ptr<Query> query, shared_ptr<DataPreprocessor> dataPreprocessor);

  vector<string> projectResult(shared_ptr<Table> resultTable,
                               shared_ptr<vector<Elem>> tuple);

  bool isInSelect(vector<string> headers);

 public:
  explicit QueryEvaluator(shared_ptr<DataRetriever> dataRetriever);

  vector<string> evaluate(shared_ptr<Query> query);
};

}  // namespace QE

#endif  // SPA_QUERYEVALUATOR_H
