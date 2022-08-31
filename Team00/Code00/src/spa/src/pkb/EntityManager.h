#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "Table.h"

using namespace std;

class EntityManager {
public:
    static Table<string> * getVariables();

private:
	static Table<string> variableTable;

};