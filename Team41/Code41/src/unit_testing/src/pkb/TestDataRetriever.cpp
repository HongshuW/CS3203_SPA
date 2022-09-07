//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/DataRetriever.h"

using namespace std;

TEST_CASE("Test Data Retriever") {
    SECTION("Get Variables in a Table") {
        DataRetriever dr;
        Table actualVariableTable = dr.getVariables();
        Table expectedVariableTable = *EntityManager::getVariables();

        REQUIRE(actualVariableTable.isEqual(expectedVariableTable));
    }

    SECTION("Get Table by Relation Type") {
        DataRetriever dr;

        Table actualFollowsTable = dr.getTableByRelationType(RelationType::FOLLOWS);
        Table actualFollowsTTable = dr.getTableByRelationType(RelationType::FOLLOWS_T);
        Table actualParentTable = dr.getTableByRelationType(RelationType::PARENT);
        Table actualParentTTable = dr.getTableByRelationType(RelationType::PARENT_T);
        Table actualUsesTable = dr.getTableByRelationType(RelationType::USES);
        Table actualModifiesTable = dr.getTableByRelationType(RelationType::MODIFIES);

        Table expectedFollowsTable = *RelationshipManager::getFollows();
        Table expectedFollowsStarTable = *RelationshipManager::getFollowsT();
        Table expectedParentTable = *RelationshipManager::getParent();
        Table expectedParentStarTable = *RelationshipManager::getParentT();
        Table expectedUsesTable = *RelationshipManager::getUses();
        Table expectedModifiesTable = *RelationshipManager::getModifies();

        REQUIRE(actualFollowsTable.isEqual(expectedFollowsTable));
        REQUIRE(actualFollowsTTable.isEqual(expectedFollowsStarTable));
        REQUIRE(actualParentTable.isEqual(expectedParentTable));
        REQUIRE(actualParentTTable.isEqual(expectedParentStarTable));
        REQUIRE(actualUsesTable.isEqual(expectedUsesTable));
        REQUIRE(actualModifiesTable.isEqual(expectedModifiesTable));
    }
}