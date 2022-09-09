//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/DataRetriever.h"

using namespace std;

TEST_CASE("Test Data Retriever") {
    SECTION("Get Table by Design Entity") {
        DataRetriever dr;

        Table actualProcedureTable = dr.getTableByDesignEntity(DesignEntity::PROCEDURE);
        Table actualStatementTable = dr.getTableByDesignEntity(DesignEntity::STMT);
        Table actualVariableTable = dr.getTableByDesignEntity(DesignEntity::VARIABLE);
        Table actualConstantTable = dr.getTableByDesignEntity(DesignEntity::CONSTANT);

        Table expectedProcedureTable = *EntityManager::getProcedures();
        Table expectedStatementTable = *EntityManager::getStatements();
        Table expectedVariableTable = *EntityManager::getVariables();
        Table expectedConstantTable = *EntityManager::getConstants();

        REQUIRE(actualProcedureTable.isEqual(expectedProcedureTable));
        REQUIRE(actualStatementTable.isEqual(expectedStatementTable));
        REQUIRE(actualVariableTable.isEqual(expectedVariableTable));
        REQUIRE(actualConstantTable.isEqual(expectedConstantTable));
    }

    SECTION("Get Table by Relation Type") {
        DataRetriever dr;

        Table actualFollowsTable = dr.getTableByRelationType(RelationType::FOLLOWS);
        Table actualFollowsTTable = dr.getTableByRelationType(RelationType::FOLLOWS_T);
        Table actualParentTable = dr.getTableByRelationType(RelationType::PARENT);
        Table actualParentTTable = dr.getTableByRelationType(RelationType::PARENT_T);
        Table actualUsesSTable = dr.getTableByRelationType(RelationType::USES_S);
        Table actualUsesPTable = dr.getTableByRelationType(RelationType::USES_P);
        Table actualModifiesSTable = dr.getTableByRelationType(RelationType::MODIFIES_S);
        Table actualModifiesPTable = dr.getTableByRelationType(RelationType::MODIFIES_S);

        Table expectedFollowsTable = *RelationshipManager::getFollows();
        Table expectedFollowsTTable = *RelationshipManager::getFollowsT();
        Table expectedParentTable = *RelationshipManager::getParent();
        Table expectedParentTTable = *RelationshipManager::getParentT();
        Table expectedUsesSTable = *RelationshipManager::getUsesS();
        Table expectedUsesPTable = *RelationshipManager::getUsesP();
        Table expectedModifiesSTable = *RelationshipManager::getModifiesS();
        Table expectedModifiesPTable = *RelationshipManager::getModifiesP();

        REQUIRE(actualFollowsTable.isEqual(expectedFollowsTable));
        REQUIRE(actualFollowsTTable.isEqual(expectedFollowsTTable));
        REQUIRE(actualParentTable.isEqual(expectedParentTable));
        REQUIRE(actualParentTTable.isEqual(expectedParentTTable));
        REQUIRE(actualUsesSTable.isEqual(expectedUsesSTable));
        REQUIRE(actualUsesPTable.isEqual(expectedUsesPTable));
        REQUIRE(actualModifiesSTable.isEqual(expectedModifiesSTable));
        REQUIRE(actualModifiesPTable.isEqual(expectedModifiesPTable));
    }
}