//
// Created by Xingchen Lin on 27/10/22.
//

#include <unordered_map>

#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/ParentRelationExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;
using namespace TestDE;

TEST_CASE("Test Parent Relation Extractor") {
  SECTION("procedure 1") {
    // procedure with no statements
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentRelationExtractor parentExtractor =
        ParentRelationExtractor(dataModifier, programNode);
    auto actualParentRelations =
        static_pointer_cast<RelationResult>(parentExtractor.extract())
            ->getResult();
    vector<vector<string>> expectedParentRelations;
    REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations,
                                                     expectedParentRelations));
  }

  SECTION("procedure 2") {
    // procedure with no parent(*) relations
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentRelationExtractor parentExtractor =
        ParentRelationExtractor(dataModifier, programNode);
    auto actualParentRelations =
        static_pointer_cast<RelationResult>(parentExtractor.extract())
            ->getResult();
    vector<vector<string>> expectedParentRelations;
    REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations,
                                                     expectedParentRelations));
  }

  SECTION("procedure 3") {
    // procedure with 2 nesting levels
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentRelationExtractor parentExtractor =
        ParentRelationExtractor(dataModifier, programNode);
    auto actualParentRelations =
        static_pointer_cast<RelationResult>(parentExtractor.extract())
            ->getResult();
    vector<string> expectedR1 = {"3", "4"};
    vector<string> expectedR2 = {"3", "5"};
    vector<string> expectedR3 = {"3", "6"};
    vector<vector<string>> expectedParentRelations{expectedR1, expectedR2,
                                                   expectedR3};
    REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations,
                                                     expectedParentRelations));
  }

  SECTION("procedure 5") {
    // procedure with 3 nesting levels
    auto programNode = TestDE::Dummies::getTestProgramNode(4);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentRelationExtractor parentExtractor =
        ParentRelationExtractor(dataModifier, programNode);
    auto actualParentRelations =
        static_pointer_cast<RelationResult>(parentExtractor.extract())
            ->getResult();
    vector<string> expectedR1 = {"2", "3"};
    vector<string> expectedR2 = {"2", "4"};
    vector<string> expectedR3 = {"4", "5"};
    vector<string> expectedR4 = {"4", "6"};
    vector<vector<string>> expectedParentRelations{expectedR1, expectedR2,
                                                   expectedR3, expectedR4};
    REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations,
                                                     expectedParentRelations));
  }

  SECTION("procedure 6") {
    // procedure with 3 nesting levels and more than 1 while loops in the same
    // nesting level
    auto programNode = TestDE::Dummies::getTestProgramNode(5);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentRelationExtractor parentExtractor =
        ParentRelationExtractor(dataModifier, programNode);
    auto actualParentRelations =
        static_pointer_cast<RelationResult>(parentExtractor.extract())
            ->getResult();
    vector<string> expectedR1 = {"2", "3"};
    vector<string> expectedR2 = {"2", "4"};
    vector<string> expectedR3 = {"4", "5"};
    vector<string> expectedR4 = {"4", "6"};
    vector<string> expectedR5 = {"8", "9"};
    vector<vector<string>> expectedParentRelations{
        expectedR1, expectedR2, expectedR3, expectedR4, expectedR5};
    REQUIRE(expectedParentRelations.size() == actualParentRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentRelations,
                                                     expectedParentRelations));
  }
}
