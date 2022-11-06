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
  void filerTableByDesignEntity(const shared_ptr<Table>& table, int colIdx,
                                DesignEntity designEntity);
  void filterTableByColValueEquality(const shared_ptr<Table>& table,
                                     const vector<int>& comparedCols);
  void filerTableByColumnIdx(const shared_ptr<Table>& table, int colIdx,
                             const string& value);
  void filterSingleClauseResultTable(Ref ref1, Ref ref2,
                                     const shared_ptr<Table>& table);
  DesignEntity getDesignEntityOfSyn(const Synonym& synonym);

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
                     const shared_ptr<vector<Value>>& values);
  bool dropUnusedColumns(const shared_ptr<Table>& table);

  shared_ptr<DataRetriever> dataRetriever;
  Declarations declarations;

 public:
  explicit DataPreprocessor(shared_ptr<DataRetriever> dataRetriever,
                            Declarations declarations);
  shared_ptr<Table> getAllByDesignEntity(DesignEntity designEntity);
  shared_ptr<Table> getNoConditionSelectClauseResult(
      const shared_ptr<SelectClause>& selectClause);
  shared_ptr<Table> getTableByAffects(
      const shared_ptr<AffectsClause>& affectsClause);
  shared_ptr<Table> getTableByAffectsT(
      const shared_ptr<AffectsTClause>& affectsTClause);
  shared_ptr<Table> getTableByCalls(const shared_ptr<CallsClause>& callsClause);
  shared_ptr<Table> getTableByCallsT(
      const shared_ptr<CallsTClause>& callsTClause);
  shared_ptr<Table> getTableByFollows(
      const shared_ptr<FollowsClause>& followsClause);
  shared_ptr<Table> getTableByFollowsT(
      const shared_ptr<FollowsTClause>& followsTClause);
  shared_ptr<Table> getTableByModifiesP(
      const shared_ptr<ModifiesPClause>& modifiesPClause);
  shared_ptr<Table> getTableByModifiesS(
      const shared_ptr<ModifiesSClause>& modifiesSClause);
  shared_ptr<Table> getTableByNext(const shared_ptr<NextClause>& nextClause);
  shared_ptr<Table> getTableByNextT(const shared_ptr<NextTClause>& nextTClause);
  shared_ptr<Table> getTableByParent(
      const shared_ptr<ParentClause>& parentClause);
  shared_ptr<Table> getTableByParentT(
      const shared_ptr<ParentTClause>& parentTClause);
  shared_ptr<Table> getTableByUsesP(const shared_ptr<UsesPClause>& usesPClause);
  shared_ptr<Table> getTableByUsesS(const shared_ptr<UsesSClause>& usesSClause);
  shared_ptr<Table> getTableByWith(const shared_ptr<WithClause>& withClause);
  shared_ptr<Table> getTableByAssignPattern(
      const shared_ptr<QB::AssignPatternClause>& assignPatternClause);
  shared_ptr<Table> getIfPatternTable(
      const shared_ptr<QB::IfPatternClause>& ifPatternClause);
  shared_ptr<Table> getWhilePatternTable(
      const shared_ptr<WhilePatternClause>& whilePatternClause);
  shared_ptr<Table> getCallsProcedureTable();
  shared_ptr<Table> getReadVariableTable();
  shared_ptr<Table> getPrintVariableTable();

  bool hasResult(const shared_ptr<WithClause>& withClause);
};

}  // namespace QE

#endif  // SPA_DATAPREPROCESSOR_H
