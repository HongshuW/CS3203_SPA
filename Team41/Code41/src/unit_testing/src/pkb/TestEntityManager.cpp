//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/EntityManager.h"

using namespace std;

TEST_CASE("Test Entity Manager") {
    SECTION("Get Variables in a Table") {
        Table * tablePtr = EntityManager::getVariables();
        REQUIRE(tablePtr == &EntityManager::variableTable);
    }

    SECTION("Save variables") {
        list<string> variables;
        variables.push_back("dummyVarA");
        variables.push_back("dummyVarB");
        int initialSize = EntityManager::variableTable.rows.size();
        EntityManager::saveVariables(variables);

        // check header is set automatically
        REQUIRE(EntityManager::variableTable.header[0] == "$variable_name");

        // check variables are added
        REQUIRE(EntityManager::variableTable.rows[initialSize][0] == "dummyVarA");
        REQUIRE(EntityManager::variableTable.rows[initialSize + 1][0] == "dummyVarB");
    }
}