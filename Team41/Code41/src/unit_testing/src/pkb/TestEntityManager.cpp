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

    SECTION("Save statements") {
        list<vector<string>> statements;
        vector<string> s1{"1", "while"};
        vector<string> s2{"2", "read"};
        statements.push_back(s1);
        statements.push_back(s2);
        int initialSize = (*EntityManager::getStatements()).rows.size();
        EntityManager::saveStatements(statements);

        // check header is set automatically
        REQUIRE((*EntityManager::getStatements()).header[0] == "$statement_number");
        REQUIRE((*EntityManager::getStatements()).header[1] == "$statement_type");

        // check statements are added
        REQUIRE((*EntityManager::getStatements()).rows[initialSize][0] == "1");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize][1] == "while");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize + 1][0] == "2");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize + 1][1] == "read");
    }
}