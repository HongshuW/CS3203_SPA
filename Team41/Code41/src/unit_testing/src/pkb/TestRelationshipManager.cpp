//
// Created by hongshu wang on 9/9/22.
//

#include "catch.hpp"
#include "pkb/RelationshipManager.h"

using namespace std;

TEST_CASE("Test Relationship Manager") {
    SECTION("Save parent") {
        int initialSize = (*RelationshipManager::getParent()).rows.size();
        vector<string> s1{"1", "2"};
        vector<string> s2{"2", "3"};
        RelationshipManager::saveParent(s1);
        RelationshipManager::saveParent(s2);

        // check header is set automatically
        REQUIRE((*RelationshipManager::getParent()).header[0] == "$parent_statement");
        REQUIRE((*RelationshipManager::getParent()).header[1] == "$child_statement");

        // check relationships are added
        REQUIRE((*RelationshipManager::getParent()).rows[initialSize][0] == "1");
        REQUIRE((*RelationshipManager::getParent()).rows[initialSize][1] == "2");
        REQUIRE((*RelationshipManager::getParent()).rows[initialSize + 1][0] == "2");
        REQUIRE((*RelationshipManager::getParent()).rows[initialSize + 1][1] == "3");
    }
}
