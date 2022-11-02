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
    Table actualProcedureTable =
        dataRetriever.getTableByDesignEntity(DesignEntity::PROCEDURE);
    Table actualStatementTable =
        dataRetriever.getTableByDesignEntity(DesignEntity::STMT);
    Table actualVariableTable =
        dataRetriever.getTableByDesignEntity(DesignEntity::VARIABLE);
    Table actualConstantTable =
        dataRetriever.getTableByDesignEntity(DesignEntity::CONSTANT);

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
    Table actualFollowsTable = dataRetriever.getFollowsTable();
    Table actualFollowsTTable = dataRetriever.getFollowsTTable();
    Table actualParentTable = dataRetriever.getParentTable();
    Table actualParentTTable = dataRetriever.getParentTTable();
    Table actualUsesSTable = dataRetriever.getUsesSTable();
    Table actualUsesPTable = dataRetriever.getUsesPTable();
    Table actualModifiesSTable = dataRetriever.getModifiesSTable();
    Table actualModifiesPTable = dataRetriever.getModifiesPTable();
    Table actualCallsTable = dataRetriever.getCallsTable();
    Table actualCallsTTable = dataRetriever.getCallsTTable();
    Table actualNextTable = dataRetriever.getNextTable();
    //        Table actualNextTTable = dataRetriever.getNextTTable();
    //        Table actualAffectsTable = dataRetriever.getAffectsTable();
    //        Table actualAffectsTTable = dataRetriever.getAffectsTTable();

    Table expectedFollowsTable = *(pkbStorage->getFollows());
    Table expectedFollowsTTable = *(pkbStorage->getFollowsT());
    Table expectedParentTable = *(pkbStorage->getParent());
    Table expectedParentTTable = *(pkbStorage->getParentT());
    Table expectedUsesSTable = *(pkbStorage->getUsesS());
    Table expectedUsesPTable = *(pkbStorage->getUsesP());
    Table expectedModifiesSTable = *(pkbStorage->getModifiesS());
    Table expectedModifiesPTable = *(pkbStorage->getModifiesP());
    Table expectedCallsTable = *(pkbStorage->getCalls());
    Table expectedCallsTTable = *(pkbStorage->getCallsT());
    Table expectedNextTable = *(pkbStorage->getNext());
    //        Table expectedNextTTable = *(pkbStorage->getNextT());
    //        Table expectedAffectsTable = *(pkbStorage->getAffects());
    //        Table expectedAffectsTTable = *(pkbStorage->getAffectsT());

    REQUIRE(actualFollowsTable.isEqual(expectedFollowsTable));
    REQUIRE(actualFollowsTTable.isEqual(expectedFollowsTTable));
    REQUIRE(actualParentTable.isEqual(expectedParentTable));
    REQUIRE(actualParentTTable.isEqual(expectedParentTTable));
    REQUIRE(actualUsesSTable.isEqual(expectedUsesSTable));
    REQUIRE(actualUsesPTable.isEqual(expectedUsesPTable));
    REQUIRE(actualModifiesSTable.isEqual(expectedModifiesSTable));
    REQUIRE(actualModifiesPTable.isEqual(expectedModifiesPTable));
    REQUIRE(actualCallsTable.isEqual(expectedCallsTable));
    REQUIRE(actualCallsTTable.isEqual(expectedCallsTTable));
    REQUIRE(actualNextTable.isEqual(expectedNextTable));
    //        REQUIRE(actualNextTTable.isEqual(expectedNextTTable));
    //        REQUIRE(actualAffectsTable.isEqual(expectedAffectsTable));
    //        REQUIRE(actualAffectsTTable.isEqual(expectedAffectsTTable));
  }

  SECTION("Get Table by Assign Pattern") {
    ExpressionSpec expressionSpec =
        ExpressionSpec(ExpressionSpecType::ANY_MATCH);
    Table actualPatternResults =
        dataRetriever.getAssignPatternTable(expressionSpec);
    Table expectedPatternResults =
        *(pkbStorage->getMatchedAssignPatterns(expressionSpec));
    Table expectedPatternTable = *(pkbStorage->getAssignPatterns());
    REQUIRE(actualPatternResults.isEqual(expectedPatternResults));
    // the results of any match should be the same as the whole pattern table
    REQUIRE(actualPatternResults.isEqual(expectedPatternTable));
  }

  SECTION("Clear cache") {
    pkbStorage->saveAffects({"1", "2"});
    pkbStorage->saveAffects({"2", "3"});
    pkbStorage->saveAffectsT({"1", "2"});
    pkbStorage->saveAffectsT({"1", "3"});
    pkbStorage->saveAffectsT({"2", "3"});
    pkbStorage->saveNextT({"1", "2"});
    pkbStorage->saveNextT({"1", "3"});
    pkbStorage->saveNextT({"2", "3"});

    dataRetriever.clearCache();

    REQUIRE(pkbStorage->getNextT()->getNumberOfRows() == 0);
    REQUIRE(pkbStorage->getAffects()->getNumberOfRows() == 0);
    REQUIRE(pkbStorage->getAffectsT()->getNumberOfRows() == 0);
  }
}