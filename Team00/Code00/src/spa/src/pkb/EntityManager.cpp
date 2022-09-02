#include <vector>

using namespace std;

#include "EntityManager.h"
#include "Table.h"

Table EntityManager::variableTable;

Table * EntityManager::getVariables() {
    // dummy table for testing
    vector<string> header{"variable name"};
    list<vector<string>> rows;
    vector<string> row1{"dummyVar1"};
    vector<string> row2{"dummyVar2"};
    vector<string> row3{"dummyVar3"};
    rows.push_back(row1);
    rows.push_back(row2);
    rows.push_back(row3);

    EntityManager::variableTable.header = header;
    EntityManager::variableTable.rows = rows;

    return &EntityManager::variableTable;
}
