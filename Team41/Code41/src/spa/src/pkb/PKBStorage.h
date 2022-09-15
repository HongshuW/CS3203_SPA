//
// Created by hongshu wang on 15/9/22.
//

#ifndef SPA_PKBSTORAGE_H
#define SPA_PKBSTORAGE_H

#include <stdio.h>
#include <list>
#include <string>
#include <vector>
#include "pkb/table/ConstantTable.h"
#include "pkb/table/Table.h"
#include "pkb/table/ProcedureTable.h"
#include "pkb/table/StatementTable.h"
#include "pkb/table/VariableTable.h"
#include "pkb/table/FollowsTable.h"
#include "pkb/table/ModifiesTable.h"
#include "pkb/table/ParentTable.h"
#include "pkb/table/Table.h"
#include "pkb/table/UsesTable.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "table/PatternTable.h"

using namespace std;

class PKBStorage {
public:
    explicit PKBStorage();

    // Column Names
    static string STATEMENT_TABLE_COL1_NAME;
    static string STATEMENT_TABLE_COL2_NAME;
    static string VARIABLE_TABLE_COL1_NAME;
    static string CONSTANT_TABLE_COL1_NAME;
    static string USES_TABLE_COL1_NAME;
    static string USES_TABLE_COL2_NAME;
    static string PROCEDURE_TABLE_COL1_NAME;
    static string PARENT_TABLE_COL1_NAME;
    static string PARENT_TABLE_COL2_NAME;
    static string MODIFIES_TABLE_COL1_NAME;
    static string MODIFIES_TABLE_COL2_NAME;
    static string FOLLOWS_TABLE_COL1_NAME;
    static string FOLLOWS_TABLE_COL2_NAME;

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
    // setters of relations
    void saveFollows(vector<string> follows);
    void saveFollowsT(vector<string> followsT);
    void saveParent(vector<string> parent);
    void saveParentT(vector<string> parentT);
    void saveUsesS(vector<string> usesS);
    void saveUsesP(vector<string> usesP);
    void saveModifiesS(vector<string> modifiesS);
    void saveModifiesP(vector<string> modifiesP);

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
    // Pattern Tables
    PatternTable patternTable;
};


#endif //SPA_PKBSTORAGE_H
