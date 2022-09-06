//
// Created by hongshu wang on 2/9/22.
//

#include "catch.hpp"
#include "pkb/DataModifier.h"
#include "pkb/EntityManager.h"

using namespace std;

TEST_CASE("Test Data Modifier") {
    SECTION ("Save variables") {
        list<string> variables{"dummyVarX", "dummyVarY", "dummyVarZ"};
        int initialSize = (*EntityManager::getVariables()).rows.size();

        DataModifier dm;
        dm.saveVariables(variables);

        // check header is set automatically
        REQUIRE((*EntityManager::getVariables()).header[0] == "$variable_name");

        // check variables are added
        REQUIRE((*EntityManager::getVariables()).rows[initialSize][0] == "dummyVarX");
        REQUIRE((*EntityManager::getVariables()).rows[initialSize + 1][0] == "dummyVarY");
        REQUIRE((*EntityManager::getVariables()).rows[initialSize + 2][0] == "dummyVarZ");
    }
}