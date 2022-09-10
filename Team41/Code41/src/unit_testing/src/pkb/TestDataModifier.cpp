//
// Created by hongshu wang on 2/9/22.
//

#include "catch.hpp"
#include "pkb/DataModifier.h"
#include "pkb/EntityManager.h"
#include "pkb/RelationshipManager.h"

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

    SECTION ("Save statements") {
        list<vector<string>> statements;
        vector<string> s1{"3", "assign"};
        vector<string> s2{"4", "if"};
        statements.push_back(s1);
        statements.push_back(s2);
        int initialSize = (*EntityManager::getStatements()).rows.size();

        DataModifier dm;
        dm.saveStatements(statements);

        // check header is set automatically
        REQUIRE((*EntityManager::getStatements()).header[0] == "$statement_number");
        REQUIRE((*EntityManager::getStatements()).header[1] == "$statement_type");

        // check statements are added
        REQUIRE((*EntityManager::getStatements()).rows[initialSize][0] == "3");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize][1] == "assign");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize + 1][0] == "4");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize + 1][1] == "if");
    }

    SECTION ("Save parentT") {
        int initialSize = (*RelationshipManager::getParentT()).rows.size();
        DataModifier dm;
        dm.saveParentT(vector<string>{"1", "2"});
        dm.saveParentT(vector<string>{"1", "3"});
        dm.saveParentT(vector<string>{"2", "3"});

        // check header is set automatically
        REQUIRE((*RelationshipManager::getParentT()).header[0] == "$parent_statement");
        REQUIRE((*RelationshipManager::getParentT()).header[1] == "$child_statement");

        // check relationships are added
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize][0] == "1");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize][1] == "2");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 1][0] == "1");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 1][1] == "3");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 2][0] == "2");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 2][1] == "3");
    }
}