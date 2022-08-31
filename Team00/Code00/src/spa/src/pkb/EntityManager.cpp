#include <vector>

using namespace std;

#include "EntityManager.h"
#include "Table.h"

Table<string> * EntityManager::getVariables() {
    // dummy table for testing
    vector<string> header{"variable name"};
    list<tuple<string>> rows;
    tuple<string> row1 = make_tuple("dummyVar1");
    tuple<string> row2 = make_tuple("dummyVar2");
    tuple<string> row3 = make_tuple("dummyVar3");
    rows.push_back(row1);
    rows.push_back(row2);
    rows.push_back(row3);

    Table<string> variableTable;
    variableTable.header = header;
    variableTable.rows = rows;

    EntityManager::variableTable = variableTable;

    return &EntityManager::variableTable;
}
