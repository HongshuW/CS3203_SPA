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
}