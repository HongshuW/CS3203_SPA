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
    static string PROCEDURE_TABLE_COL1_NAME;
    static string STATEMENT_TABLE_COL1_NAME;
    static string STATEMENT_TABLE_COL2_NAME;
    static string VARIABLE_TABLE_COL1_NAME;
    static string CONSTANT_TABLE_COL1_NAME;
    static string USES_TABLE_COL1_NAME;
    static string USES_TABLE_COL2_NAME;
    static string PARENT_TABLE_COL1_NAME;
    static string PARENT_TABLE_COL2_NAME;
    static string MODIFIES_TABLE_COL1_NAME;
    static string MODIFIES_TABLE_COL2_NAME;
    static string FOLLOWS_TABLE_COL1_NAME;
    static string FOLLOWS_TABLE_COL2_NAME;
    static string CALLS_TABLE_COL1_NAME;
    static string CALLS_TABLE_COL2_NAME;
    static string NEXT_TABLE_COL1_NAME;
    static string NEXT_TABLE_COL2_NAME;
    static string AFFECTS_TABLE_COL1_NAME;
    static string AFFECTS_TABLE_COL2_NAME;

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
    CallsTable * getCallsTable();
    CallsTable * getCallsTTable();
    NextTable * getNextTable();
    NextTable * getNextTTable();
    AffectsTable * getAffectsTable();
    AffectsTable * getAffectsTTable();
    // setters of relations
    void saveFollows(vector<string> follows);
    void saveFollowsT(vector<string> followsT);
    void saveParent(vector<string> parent);
    void saveParentT(vector<string> parentT);
    void saveUsesS(vector<string> usesS);
    void saveUsesP(vector<string> usesP);
    void saveModifiesS(vector<string> modifiesS);
    void saveModifiesP(vector<string> modifiesP);
    void saveCallsTable(vector<string> calls);
    void saveCallsTTable(vector<string> callsT);
    void saveNextTable(vector<string> next);
    void saveNextTTable(vector<string> nextT);
    void saveAffectsTable(vector<string> affects);
    void saveAffectsTTable(vector<string> affectsT);

    // getters of patterns
    shared_ptr<Table> getMatchedPatterns(ExpressionSpec expressionSpec);
    PatternTable * getPatterns();
    // setters of patterns
    void savePattern(vector<string> metainfo, shared_ptr<ExprNode> pattern);

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
    PatternTable patternTable;
};


#endif //SPA_PKBSTORAGE_H
