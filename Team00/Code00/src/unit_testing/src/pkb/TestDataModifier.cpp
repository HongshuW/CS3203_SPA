//
// Created by hongshu wang on 2/9/22.
//

#include "catch.hpp"
#include "pkb/DataModifier.h"
#include "pkb/EntityManager.h"

using namespace std;

TEST_CASE("Test Data Modifier") {
    SECTION ("Save variables") {
        list<string> variables;
        variables.push_back("dummyVarX");
        variables.push_back("dummyVarY");
        variables.push_back("dummyVarZ");
        int initialSize = EntityManager::variableTable.rows.size();

        DataModifier dm;
        dm.saveVariables(variables);

        // check header is set automatically
        REQUIRE(EntityManager::variableTable.header[0] == "variable name");

        // check variables are added
        REQUIRE(EntityManager::variableTable.rows[initialSize][0] == "dummyVarX");
        REQUIRE(EntityManager::variableTable.rows[initialSize + 1][0] == "dummyVarY");
        REQUIRE(EntityManager::variableTable.rows[initialSize + 2][0] == "dummyVarZ");
    }
}