#pragma once

#include <stdio.h>
#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "Table.h"

using namespace std;

class EntityManager {
public:
    static Table * getProcedures();
    static Table * getStatements();
    static Table * getVariables();
    static Table * getConstants();
    static void saveVariables(list<string> variables);

private:
    static Table procedureTable;
    static Table statementTable;
    static Table variableTable;
    static Table constantTable;

    static vector<string> procedureTableHeader;
    static vector<string> statementTableHeader;
    static vector<string> variableTableHeader;
    static vector<string> constantTableHeader;

    static void setHeader(Table * table, vector<string> header);
};