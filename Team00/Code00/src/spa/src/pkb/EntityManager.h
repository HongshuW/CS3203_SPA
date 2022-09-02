#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Table.h"

using namespace std;

class EntityManager {
public:
    static Table variableTable;

    static Table * getVariables();

};