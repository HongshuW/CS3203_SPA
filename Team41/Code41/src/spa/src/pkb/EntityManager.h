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

    // setters
    static void saveVariables(list<string> variables);

private:
    static ProcedureTable procedureTable;
    static StatementTable statementTable;
    static VariableTable variableTable;
    static ConstantTable constantTable;

    static void setHeader(Table * table, vector<string> header);
};