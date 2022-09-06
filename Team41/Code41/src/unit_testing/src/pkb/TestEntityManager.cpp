//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/EntityManager.h"

using namespace std;

TEST_CASE("Test Entity Manager") {
    SECTION("Save variables") {
        list<string> variables{"dummyVarA", "dummyVarB"};
        int initialSize = (*EntityManager::getVariables()).rows.size();
        EntityManager::saveVariables(variables);

        // check header is set automatically
        REQUIRE((*EntityManager::getVariables()).header[0] == "$variable_name");

        // check variables are added
        REQUIRE((*EntityManager::getVariables()).rows[initialSize][0] == "dummyVarA");
        REQUIRE((*EntityManager::getVariables()).rows[initialSize + 1][0] == "dummyVarB");
    }
}