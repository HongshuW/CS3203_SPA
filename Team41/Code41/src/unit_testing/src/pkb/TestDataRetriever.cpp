//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/DataRetriever.h"
#include "pkb/exception/KeyNotFoundException.h"

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

    SECTION ("Get Design Entity of Statement") {
        DataRetriever dr;
        // add valid rows to statement table
        vector<string> r1{"100", "if"};
        vector<string> r2{"101", "assign"};
        EntityManager::getStatements()->appendRow(r1);
        EntityManager::getStatements()->appendRow(r2);
        // test valid rows
        DesignEntity actualRow1DE = dr.getDesignEntityOfStmt(100);
        DesignEntity actualRow2DE = dr.getDesignEntityOfStmt(101);
        DesignEntity expectedRow1DE = DesignEntity::IF;
        DesignEntity expectedRow2DE = DesignEntity::ASSIGN;
        REQUIRE(actualRow1DE == expectedRow1DE);
        REQUIRE(actualRow2DE == expectedRow2DE);
        // test invalid keys
        REQUIRE_THROWS(dr.getDesignEntityOfStmt(1000));
    }
}