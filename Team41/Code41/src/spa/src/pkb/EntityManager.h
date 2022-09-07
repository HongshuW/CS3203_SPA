#pragma once

#include <stdio.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "pkb/table/ConstantTable.h"
#include "pkb/table/Table.h"
#include "pkb/table/ProcedureTable.h"
#include "pkb/table/StatementTable.h"
#include "pkb/table/VariableTable.h"

using namespace std;

class EntityManager {
public:
    // getters
    static ProcedureTable * getProcedures();
    static StatementTable * getStatements();
    static VariableTable * getVariables();
    static ConstantTable * getConstants();

    static string getStmtType(string stmtNumber);

    // setters
    static void saveProcedures(list<string> procedures);
    static void saveStatements(list<vector<string>> statements);
    static void saveVariables(list<string> variables);
    static void saveConstants(list<string> constants);

    static void setHeader(Table * table, vector<string> header);

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

private:
    static ProcedureTable procedureTable;
    static StatementTable statementTable;
    static VariableTable variableTable;
    static ConstantTable constantTable;

};