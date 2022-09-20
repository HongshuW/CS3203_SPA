//
// Created by hongshu wang on 1/9/22.
//

#include "catch.hpp"
#include "pkb/DataRetriever.h"
#include "pkb/exception/KeyNotFoundException.h"

using namespace std;

TEST_CASE("Test Data Retriever") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
    DataRetriever dataRetriever = DataRetriever(pkbStorage);

    SECTION("Get Table by Design Entity") {
        Table actualProcedureTable = dataRetriever.getTableByDesignEntity(DesignEntity::PROCEDURE);
        Table actualStatementTable = dataRetriever.getTableByDesignEntity(DesignEntity::STMT);
        Table actualVariableTable = dataRetriever.getTableByDesignEntity(DesignEntity::VARIABLE);
        Table actualConstantTable = dataRetriever.getTableByDesignEntity(DesignEntity::CONSTANT);

        Table expectedProcedureTable = *(pkbStorage->getProcedures());
        Table expectedStatementTable = *(pkbStorage->getStatements());
        Table expectedVariableTable = *(pkbStorage->getVariables());
        Table expectedConstantTable = *(pkbStorage->getConstants());

        REQUIRE(actualProcedureTable.isEqual(expectedProcedureTable));
        REQUIRE(actualStatementTable.isEqual(expectedStatementTable));
        REQUIRE(actualVariableTable.isEqual(expectedVariableTable));
        REQUIRE(actualConstantTable.isEqual(expectedConstantTable));
    }

    SECTION("Get Table by Relation Type") {
        Table actualFollowsTable = dataRetriever.getTableByRelationType(RelationType::FOLLOWS);
        Table actualFollowsTTable = dataRetriever.getTableByRelationType(RelationType::FOLLOWS_T);
        Table actualParentTable = dataRetriever.getTableByRelationType(RelationType::PARENT);
        Table actualParentTTable = dataRetriever.getTableByRelationType(RelationType::PARENT_T);
        Table actualUsesSTable = dataRetriever.getTableByRelationType(RelationType::USES_S);
        Table actualUsesPTable = dataRetriever.getTableByRelationType(RelationType::USES_P);
        Table actualModifiesSTable = dataRetriever.getTableByRelationType(RelationType::MODIFIES_S);
        Table actualModifiesPTable = dataRetriever.getTableByRelationType(RelationType::MODIFIES_S);

        Table expectedFollowsTable = *(pkbStorage->getFollows());
        Table expectedFollowsTTable = *(pkbStorage->getFollowsT());
        Table expectedParentTable = *(pkbStorage->getParent());
        Table expectedParentTTable = *(pkbStorage->getParentT());
        Table expectedUsesSTable = *(pkbStorage->getUsesS());
        Table expectedUsesPTable = *(pkbStorage->getUsesP());
        Table expectedModifiesSTable = *(pkbStorage->getModifiesS());
        Table expectedModifiesPTable = *(pkbStorage->getModifiesP());

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
        // add valid rows to statement table
        vector<string> r1{"100", "if"};
        vector<string> r2{"101", "assign"};
        StatementTable * statementTable = pkbStorage->getStatements();
        statementTable->appendRow(r1);
        statementTable->appendRow(r2);
        // test valid rows
        DesignEntity actualRow1DE = dataRetriever.getDesignEntityOfStmt(100);
        DesignEntity actualRow2DE = dataRetriever.getDesignEntityOfStmt(101);
        DesignEntity expectedRow1DE = DesignEntity::IF;
        DesignEntity expectedRow2DE = DesignEntity::ASSIGN;
        REQUIRE(actualRow1DE == expectedRow1DE);
        REQUIRE(actualRow2DE == expectedRow2DE);
        // test invalid keys
        REQUIRE_THROWS(dataRetriever.getDesignEntityOfStmt(1000));
    }

    SECTION ("Get Table by Assign Pattern") {
        ExpressionSpec expressionSpec = ExpressionSpec(ExpressionSpecType::ANY_MATCH);
        Table actualPatternResults = dataRetriever.getTableByPattern(expressionSpec);
        Table expectedPatternResults = *(pkbStorage->getMatchedAssignPatterns(expressionSpec));
        Table expectedPatternTable = *(pkbStorage->getAssignPatterns());
        REQUIRE(actualPatternResults.isEqual(expectedPatternResults));
        // the results of any match should be the same as the whole pattern table
        REQUIRE(actualPatternResults.isEqual(expectedPatternTable));
    }
}