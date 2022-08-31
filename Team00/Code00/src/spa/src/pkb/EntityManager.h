#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Table.h"

using namespace std;

class EntityManager {
public:
    static Table<string> variableTable;
    static Table<string> * getVariables();

};