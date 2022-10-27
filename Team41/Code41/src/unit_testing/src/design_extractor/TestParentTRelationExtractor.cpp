//
// Created by Xingchen Lin on 27/10/22.
//

#include <unordered_map>

#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/ParentTRelationExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;
using namespace TestDE;

TEST_CASE("Test ParentT Relation Extractor") {
  SECTION("procedure 1") {
    // procedure with no statements
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentTRelationExtractor parentTExtractor =
        ParentTRelationExtractor(dataModifier, programNode);
    auto actualParentTRelations =
        static_pointer_cast<RelationResult>(parentTExtractor.extract())
            ->getResult();
    vector<vector<string>> expectedParentTRelations;
    REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations,
                                                     expectedParentTRelations));
  }

  SECTION("procedure 2") {
    // procedure with no parent(*) relations
    auto programNode = TestDE::Dummies::getTestProgramNode(1);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentTRelationExtractor parentTExtractor =
        ParentTRelationExtractor(dataModifier, programNode);
    auto actualParentTRelations =
        static_pointer_cast<RelationResult>(parentTExtractor.extract())
            ->getResult();
    vector<vector<string>> expectedParentTRelations;
    REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations,
                                                     expectedParentTRelations));
  }

  SECTION("procedure 3") {
    // procedure with 2 nesting levels
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentTRelationExtractor parentTExtractor =
        ParentTRelationExtractor(dataModifier, programNode);
    auto actualParentTRelations =
        static_pointer_cast<RelationResult>(parentTExtractor.extract())
            ->getResult();
    vector<string> expectedR1 = {"3", "4"};
    vector<string> expectedR2 = {"3", "5"};
    vector<string> expectedR3 = {"3", "6"};
    vector<vector<string>> expectedParentTRelations{expectedR1, expectedR2,
                                                    expectedR3};
    REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations,
                                                     expectedParentTRelations));
  }

  SECTION("procedure 5") {
    // procedure with 3 nesting levels
    auto programNode = TestDE::Dummies::getTestProgramNode(4);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentTRelationExtractor parentTExtractor =
        ParentTRelationExtractor(dataModifier, programNode);
    auto actualParentTRelations =
        static_pointer_cast<RelationResult>(parentTExtractor.extract())
            ->getResult();
    vector<string> expectedR1 = {"2", "3"};
    vector<string> expectedR2 = {"2", "4"};
    vector<string> expectedR3 = {"4", "5"};
    vector<string> expectedR4 = {"4", "6"};
    vector<string> expectedR5 = {"2", "5"};
    vector<string> expectedR6 = {"2", "6"};
    vector<vector<string>> expectedParentTRelations{
        expectedR1, expectedR2, expectedR5, expectedR6, expectedR3, expectedR4};
    REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations,
                                                     expectedParentTRelations));
  }

  SECTION("procedure 6") {
    // procedure with 3 nesting levels and more than 1 while loops in the same
    // nesting level
    auto programNode = TestDE::Dummies::getTestProgramNode(5);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    ParentTRelationExtractor parentTExtractor =
        ParentTRelationExtractor(dataModifier, programNode);
    auto actualParentTRelations =
        static_pointer_cast<RelationResult>(parentTExtractor.extract())
            ->getResult();
    vector<string> expectedR1 = {"2", "3"};
    vector<string> expectedR2 = {"2", "4"};
    vector<string> expectedR3 = {"4", "5"};
    vector<string> expectedR4 = {"4", "6"};
    vector<string> expectedR5 = {"8", "9"};
    vector<string> expectedR6 = {"2", "5"};
    vector<string> expectedR7 = {"2", "6"};
    vector<vector<string>> expectedParentTRelations{
        expectedR1, expectedR2, expectedR3, expectedR4,
        expectedR5, expectedR6, expectedR7};
    REQUIRE(expectedParentTRelations.size() == actualParentTRelations->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actualParentTRelations,
                                                     expectedParentTRelations));
  }
}
