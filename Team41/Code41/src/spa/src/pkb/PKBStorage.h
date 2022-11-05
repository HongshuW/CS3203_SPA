//
// Created by hongshu wang on 15/9/22.
//

#ifndef SPA_PKBSTORAGE_H
#define SPA_PKBSTORAGE_H

#include <stdio.h>

#include <list>
#include <string>
#include <vector>

#include "query_builder/commons/ExpressionSpec.h"
#include "table/AffectsTable.h"
#include "table/CallsTable.h"
#include "table/ConstantTable.h"
#include "table/FollowsTable.h"
#include "table/ModifiesTable.h"
#include "table/NextTable.h"
#include "table/ParentTable.h"
#include "table/PatternTable.h"
#include "table/ProcedureTable.h"
#include "table/StatementTable.h"
#include "table/Table.h"
#include "table/UsesTable.h"
#include "table/VariableTable.h"

using namespace std;

class PKBStorage {
 public:
  explicit PKBStorage();

  // Column Names
  static const string PROCEDURE_TABLE_COL1_NAME;
  static const string PROCEDURE_TABLE_COL2_NAME;
  static const string STATEMENT_TABLE_COL1_NAME;
  static const string STATEMENT_TABLE_COL2_NAME;
  static const string VARIABLE_TABLE_COL1_NAME;
  static const string CONSTANT_TABLE_COL1_NAME;
  static const string USES_TABLE_COL1_NAME;
  static const string USES_TABLE_COL2_NAME;
  static const string PARENT_TABLE_COL1_NAME;
  static const string PARENT_TABLE_COL2_NAME;
  static const string MODIFIES_TABLE_COL1_NAME;
  static const string MODIFIES_TABLE_COL2_NAME;
  static const string FOLLOWS_TABLE_COL1_NAME;
  static const string FOLLOWS_TABLE_COL2_NAME;
  static const string CALLS_TABLE_COL1_NAME;
  static const string CALLS_TABLE_COL2_NAME;
  static const string NEXT_TABLE_COL1_NAME;
  static const string NEXT_TABLE_COL2_NAME;
  static const string AFFECTS_TABLE_COL1_NAME;
  static const string AFFECTS_TABLE_COL2_NAME;

  // getters of entities
  shared_ptr<ProcedureTable> getProcedures();
  shared_ptr<Table> getStatements();
  shared_ptr<Table> getPrintVariableNames();
  shared_ptr<Table> getReadVariableNames();
  shared_ptr<VariableTable> getVariables();
  shared_ptr<ConstantTable> getConstants();
  string getStmtType(string stmtNumber);
  // setters of entities
  void saveProcedures(list<string> procedures);
  void saveStatements(list<vector<string>> statements);
  void saveVariables(list<string> variables);
  void saveConstants(list<string> constants);

  // getters of relations
  shared_ptr<FollowsTable> getFollows();
  shared_ptr<FollowsTable> getFollowsT();
  shared_ptr<ParentTable> getParent();
  shared_ptr<ParentTable> getParentT();
  shared_ptr<UsesTable> getUsesS();
  shared_ptr<UsesTable> getUsesP();
  shared_ptr<ModifiesTable> getModifiesS();
  shared_ptr<ModifiesTable> getModifiesP();
  shared_ptr<Table> getCalls();
  shared_ptr<CallsTable> getCallsT();
  shared_ptr<Table> getCallsProcedureNames();
  shared_ptr<NextTable> getNext();
  shared_ptr<NextTable> getNextT();
  shared_ptr<AffectsTable> getAffects();
  shared_ptr<AffectsTable> getAffectsT();
  // setters of relations
  void saveFollows(vector<string> follows);
  void saveFollowsT(vector<string> followsT);
  void saveParent(vector<string> parent);
  void saveParentT(vector<string> parentT);
  void saveUsesS(vector<string> usesS);
  void saveUsesP(vector<string> usesP);
  void saveModifiesS(vector<string> modifiesS);
  void saveModifiesP(vector<string> modifiesP);
  void saveCalls(vector<string> calls);
  void saveCallsT(vector<string> callsT);
  void saveNext(vector<string> next);
  void saveNextT(vector<string> nextT);
  void saveAffects(vector<string> affects);
  void saveAffectsT(vector<string> affectsT);

  // getters of patterns
  shared_ptr<Table> getMatchedAssignPatterns(ExpressionSpec expressionSpec);
  shared_ptr<PatternTable> getAssignPatterns();
  shared_ptr<PatternTable> getWhilePatterns();
  shared_ptr<PatternTable> getIfPatterns();
  // setters of patterns
  void saveAssignPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern);
  void saveWhilePattern(vector<string> metainfo);
  void saveIfPattern(vector<string> metainfo);

  void clearCache();

 private:
  // Entity Tables
  shared_ptr<ProcedureTable> procedureTable;
  shared_ptr<StatementTable> statementTable;
  shared_ptr<VariableTable> variableTable;
  shared_ptr<ConstantTable> constantTable;
  // Relation Tables
  shared_ptr<FollowsTable> followsTable;
  shared_ptr<FollowsTable> followsTTable;
  shared_ptr<ParentTable> parentTable;
  shared_ptr<ParentTable> parentTTable;
  shared_ptr<UsesTable> usesSTable;
  shared_ptr<UsesTable> usesPTable;
  shared_ptr<ModifiesTable> modifiesSTable;
  shared_ptr<ModifiesTable> modifiesPTable;
  shared_ptr<CallsTable> callsTable;
  shared_ptr<CallsTable> callsTTable;
  shared_ptr<NextTable> nextTable;
  shared_ptr<NextTable> nextTTable;
  shared_ptr<AffectsTable> affectsTable;
  shared_ptr<AffectsTable> affectsTTable;
  // Pattern Tables
  shared_ptr<PatternTable> assignPatternTable;
  shared_ptr<PatternTable> whilePatternTable;
  shared_ptr<PatternTable> ifPatternTable;
};

#endif  // SPA_PKBSTORAGE_H
