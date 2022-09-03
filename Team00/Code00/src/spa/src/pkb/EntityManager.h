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
    static Table variableTable;

    static Table * getVariables();
    static void saveVariables(list<string> variables);

private:
    static void setHeader(Table * table, vector<string> header);
}; 