#include <vector>

using namespace std;

#include "EntityManager.h"

Table<string> * EntityManager::getVariables() {
    return &EntityManager::variableTable;
}
