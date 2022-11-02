//
// Created by Nafour on 5/9/22.
//

#ifndef SPA_DATAPREPROCESSOR_H
#define SPA_DATAPREPROCESSOR_H

#include <memory>

#include "pkb/DataRetriever.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/clauses/such_that_clauses/SuchThatClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"
#include "query_builder/commons/Declaration.h"
#include "query_builder/commons/DesignEntity.h"

using namespace std;
using namespace QB;

typedef shared_ptr<vector<Declaration>> Declarations;
namespace QE {
using Value = variant<string, int, vector<int>, vector<string>>;

class DataPreprocessor {
 private:
  void filerTableByDesignEntity(shared_ptr<Table> table, int colIdx,
                                DesignEntity designEntity);
  void filterTableByColValueEquality(shared_ptr<Table> table,
                                     const vector<int>& comparedCols);
  void filerTableByColumnIdx(shared_ptr<Table> table, int colIdx, const string& value);
  void filterSingleClauseResultTable(Ref ref1, Ref ref2, shared_ptr<Table> table);
  DesignEntity getDesignEntityOfSyn(Synonym synonym);

  /**
   * Used for with clause evaluation
   * Valid paras: stmt, read, print, call, while, if assign
   * @param designEntity
   * @return
   */
  vector<int> getStmtNumsByDesignEntity(DesignEntity designEntity);

  /**
   * Used for with clause evaluation
   * Valid paras: procedure, variable, call, read, print
   * @param designEntity
   * @return
   */
  vector<string> getEntityNames(DesignEntity designEntity);
  std::vector<std::string> intersection(std::vector<std::string> v1,
                                        std::vector<std::string> v2);
  std::vector<int> intersection(std::vector<int> v1, std::vector<int> v2);
  void getWithValues(vector<WithRef> withRefs,
                     shared_ptr<vector<Value>> values);
  bool dropUnusedColumns(shared_ptr<Table> table);

  shared_ptr<DataRetriever> dataRetriever;
  Declarations declarations;

 public:
  explicit DataPreprocessor(shared_ptr<DataRetriever> dataRetriever,
                            Declarations declarations);
  shared_ptr<Table> getAllByDesignEntity(DesignEntity designEntity);
  shared_ptr<Table> getNoConditionSelectClauseResult(shared_ptr<SelectClause> selectClause);
  shared_ptr<Table> getTableByAffects(shared_ptr<AffectsClause> affectsClause);
  shared_ptr<Table> getTableByAffectsT(shared_ptr<AffectsTClause> affectsTClause);
  shared_ptr<Table> getTableByCalls(shared_ptr<CallsClause> callsClause);
  shared_ptr<Table> getTableByCallsT(shared_ptr<CallsTClause> callsTClause);
  shared_ptr<Table> getTableByFollows(shared_ptr<FollowsClause> followsClause);
  shared_ptr<Table> getTableByFollowsT(shared_ptr<FollowsTClause> followsTClause);
  shared_ptr<Table> getTableByModifiesP(shared_ptr<ModifiesPClause> modifiesPClause);
  shared_ptr<Table> getTableByModifiesS(shared_ptr<ModifiesSClause> modifiesSClause);
  shared_ptr<Table> getTableByNext(shared_ptr<NextClause> nextClause);
  shared_ptr<Table> getTableByNextT(shared_ptr<NextTClause> nextTClause);
  shared_ptr<Table> getTableByParent(shared_ptr<ParentClause> parentClause);
  shared_ptr<Table> getTableByParentT(shared_ptr<ParentTClause> parentTClause);
  shared_ptr<Table> getTableByUsesP(shared_ptr<UsesPClause> usesPClause);
  shared_ptr<Table> getTableByUsesS(shared_ptr<UsesSClause> usesSClause);
  shared_ptr<Table> getTableByWith(shared_ptr<WithClause> withClause);
  shared_ptr<Table> getTableByAssignPattern(
      shared_ptr<QB::AssignPatternClause> assignPatternClause);
  shared_ptr<Table> getIfPatternTable(shared_ptr<QB::IfPatternClause> ifPatternClause);
  shared_ptr<Table> getWhilePatternTable(shared_ptr<WhilePatternClause> whilePatternClause);
  shared_ptr<Table> getCallsProcedureTable();
  shared_ptr<Table> getReadVariableTable();
  shared_ptr<Table> getPrintVariableTable();

  bool hasResult(shared_ptr<WithClause> withClause);
};

}  // namespace QE

#endif  // SPA_DATAPREPROCESSOR_H
