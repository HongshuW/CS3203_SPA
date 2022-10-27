//
// Created by Xingchen Lin on 27/10/22.
//

#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/CallsTRelationExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test CallsT Relation Extractor") {
  SECTION("Test Empty Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    CallsTRelationExtractor callsTExtractor =
        CallsTRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(callsTExtractor.extract())
                      ->getResult();
    REQUIRE(actual->empty());
  }

  SECTION("Test CallStar One Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(10);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    CallsTRelationExtractor callsTExtractor =
        CallsTRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(callsTExtractor.extract())
                      ->getResult();
    vector<vector<string>> expected = {{"procedure2", "procedure3"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test CallStar In Singly-Nested Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(12);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    CallsTRelationExtractor callsTExtractor =
        CallsTRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(callsTExtractor.extract())
                      ->getResult();
    vector<vector<string>> expected = {{"procedure2", "procedure3"},
                                       {"procedure2", "procedure4"},
                                       {"procedure2", "procedure6"},
                                       {"procedure3", "procedure4"},
                                       {"procedure3", "procedure6"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }

  SECTION("Test CallStar In Doubly-Nested Procedure") {
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    auto programNode = TestDE::Dummies::getTestProgramNode(13);
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    CallsTRelationExtractor callsTExtractor =
        CallsTRelationExtractor(dataModifier, programNode);
    auto actual = static_pointer_cast<RelationResult>(callsTExtractor.extract())
                      ->getResult();
    vector<vector<string>> expected = {
        {"procedure2", "procedure3"}, {"procedure2", "procedure4"},
        {"procedure2", "procedure5"}, {"procedure2", "procedure6"},
        {"procedure3", "procedure5"}, {"procedure4", "procedure6"}};
    REQUIRE(expected.size() == actual->size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(*actual, expected));
  }
}