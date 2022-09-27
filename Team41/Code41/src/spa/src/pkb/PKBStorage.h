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
    ProcedureTable * getProcedures();
    StatementTable * getStatements();
    VariableTable * getVariables();
    ConstantTable * getConstants();
    string getStmtType(string stmtNumber);
    // setters of entities
    void saveProcedures(list<string> procedures);
    void saveStatements(list<vector<string>> statements);
    void saveVariables(list<string> variables);
    void saveConstants(list<string> constants);

    // getters of relations
    FollowsTable * getFollows();
    FollowsTable * getFollowsT();
    ParentTable * getParent();
    ParentTable * getParentT();
    UsesTable * getUsesS();
    UsesTable * getUsesP();
    ModifiesTable * getModifiesS();
    ModifiesTable * getModifiesP();
    CallsTable * getCalls();
    CallsTable * getCallsT();
    NextTable * getNext();
    NextTable * getNextT();
    AffectsTable * getAffects();
    AffectsTable * getAffectsT();
    shared_ptr<unordered_set<string>> getFollowingStatements(string followedStatement);
    shared_ptr<unordered_set<string>> getChildrenStatements(string parentStatement);
    shared_ptr<unordered_set<string>> getModifiedVariables(string modifierStatement);
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
    PatternTable * getAssignPatterns();
    PatternTable * getWhilePatterns();
    PatternTable * getIfPatterns();
    // setters of patterns
    void saveAssignPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern);
    void saveWhilePattern(vector<string> metainfo);
    void saveIfPattern(vector<string> metainfo);

private:
    // Entity Tables
    ProcedureTable procedureTable;
    StatementTable statementTable;
    VariableTable variableTable;
    ConstantTable constantTable;
    // Relation Tables
    FollowsTable followsTable;
    FollowsTable followsTTable;
    ParentTable parentTable;
    ParentTable parentTTable;
    UsesTable usesSTable;
    UsesTable usesPTable;
    ModifiesTable modifiesSTable;
    ModifiesTable modifiesPTable;
    CallsTable callsTable;
    CallsTable callsTTable;
    NextTable nextTable;
    NextTable nextTTable;
    AffectsTable affectsTable;
    AffectsTable affectsTTable;
    // Pattern Tables
    PatternTable assignPatternTable;
    PatternTable whilePatternTable;
    PatternTable ifPatternTable;
};


#endif //SPA_PKBSTORAGE_H
