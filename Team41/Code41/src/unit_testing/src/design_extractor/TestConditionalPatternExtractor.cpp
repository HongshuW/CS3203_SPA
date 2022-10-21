//
// Created by Aaron on 8/10/22.
//
#include <unordered_map>

#include "AST/IfNode.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include "DEUtils.h"
#include "Dummies.h"
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/PatternExtractor.h"
#include "iostream"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test Conditional Pattern Extractor") {
  SECTION("test empty procedure for extract if") {
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    list<vector<string>> actual =
        PatternExtractor::extractIfPattern(programNode);
    REQUIRE(actual.empty());
  }

  SECTION("test empty procedure for extract while") {
    auto programNode = TestDE::Dummies::getTestProgramNode(0);
    list<vector<string>> actual =
        PatternExtractor::extractWhilePattern(programNode);
    REQUIRE(actual.empty());
  }

  SECTION("test simple if statement") {
    auto programNode = TestDE::Dummies::getTestProgramNode(2);
    list<vector<string>> actual =
        PatternExtractor::extractIfPattern(programNode);
    vector<vector<string>> expected = {{"3", "bar"}, {"3", "y"}};
    REQUIRE(expected.size() == actual.size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
  }

  SECTION("test complicated while statement") {
    auto programNode = TestDE::Dummies::getTestProgramNode(3);
    list<vector<string>> actual =
        PatternExtractor::extractWhilePattern(programNode);
    vector<vector<string>> expected = {
        {"5", "baz"}, {"5", "qux"}, {"5", "quux"}, {"5", "haha"}};
    REQUIRE(expected.size() == actual.size());
    REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
  }
}