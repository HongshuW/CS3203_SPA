//
// Created by Aaron on 8/10/22.
//
#include <unordered_map>

#include "AST/IfNode.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/extractors/IfPatternExtractor.h"
#include "design_extractor/extractors/WhilePatternExtractor.h"
#include "design_extractor/results/RelationResult.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Conditional Pattern Extractor") {
  SECTION("test empty procedure for extract if") {
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    IfPatternExtractor ifPatternExtractor =
        IfPatternExtractor(dataModifier, programNode);
    auto actual =
        *static_pointer_cast<RelationResult>(ifPatternExtractor.extract())
             ->getResult();
    REQUIRE(actual.empty());
  }

  SECTION("test empty procedure for extract while") {
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    WhilePatternExtractor whilePatternExtractor =
        WhilePatternExtractor(dataModifier, programNode);
    auto actual =
        *static_pointer_cast<RelationResult>(whilePatternExtractor.extract())
             ->getResult();
    REQUIRE(actual.empty());
  }

  SECTION("test simple if statement") {
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    IfPatternExtractor ifPatternExtractor =
        IfPatternExtractor(dataModifier, programNode);
    auto actual =
        *static_pointer_cast<RelationResult>(ifPatternExtractor.extract())
             ->getResult();
    vector<vector<string>> expected = {{"3", "bar"}, {"3", "y"}};
    REQUIRE(expected.size() == actual.size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
  }

  SECTION("test complicated while statement") {
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>();
    shared_ptr<DataModifier> dataModifier =
        make_shared<DataModifier>(pkbStorage);
    WhilePatternExtractor whilePatternExtractor =
        WhilePatternExtractor(dataModifier, programNode);
    auto actual =
        *static_pointer_cast<RelationResult>(whilePatternExtractor.extract())
             ->getResult();
    vector<vector<string>> expected = {
        {"5", "baz"}, {"5", "qux"}, {"5", "quux"}, {"5", "haha"}};
    REQUIRE(expected.size() == actual.size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
  }
}