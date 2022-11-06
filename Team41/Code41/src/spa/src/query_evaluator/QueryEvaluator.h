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

  DesignEntity getDesignEntity(const Synonym& synonym);

  vector<string> evaluateNoConditionSelectTupleQuery(
      const shared_ptr<Query>& sharedPtr,
      const shared_ptr<ConcreteClauseVisitor>& clauseVisitor);

  vector<string> evaluateSelectBoolQuery(
      const shared_ptr<ConcreteClauseVisitor>& clauseVisitor,
      const shared_ptr<DataPreprocessor>& dataPreprocessor,
      const ConnectedClauseGroups& ccg);

  vector<string> evaluateSelectTupleQuery(
      const shared_ptr<ConcreteClauseVisitor>& clauseVisitor,
      const shared_ptr<DataPreprocessor>& dataPreprocessor,
      const ConnectedClauseGroups& ccg);

  vector<string> removeDup(vector<string> vec);

  vector<string> formatConditionalQueryResult(
      shared_ptr<Table> resultTable, const shared_ptr<vector<Elem>>& tuple,
      const shared_ptr<Query>& sharedPtr,
      const shared_ptr<DataPreprocessor>& dataPreprocessor);

  vector<string> projectResult(const shared_ptr<Table>& resultTable,
                               const shared_ptr<vector<Elem>>& tuple);

  bool isInSelect(vector<string> headers);

 public:
  explicit QueryEvaluator(shared_ptr<DataRetriever> dataRetriever);

  vector<string> evaluate(const shared_ptr<Query>& queryPtr);
};

}  // namespace QE

#endif  // SPA_QUERYEVALUATOR_H
