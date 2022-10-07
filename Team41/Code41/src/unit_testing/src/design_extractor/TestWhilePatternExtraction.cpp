//
// Created by Aaron on 8/10/22.
//
#include "catch.hpp"
#include "design_extractor/DesignExtractor.h"
#include "design_extractor/PatternExtractor.h"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/IfNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include <unordered_map>
#include "pkb/DataModifier.h"
#include "iostream"
#include "Dummies.h"
#include "DEUtils.h"

using namespace std;
using namespace DE;

TEST_CASE("Test While Pattern Extractor") {
    SECTION("test empty procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        list<vector<string>> actual = PatternExtractor::extractIfPattern(programNode);
        REQUIRE(actual.empty());
    }

    SECTION("test while statement") {
        auto programNode = TestDE::Dummies::getTestProgramNode(3);
        list<vector<string>> actual = PatternExtractor::extractWhilePattern(programNode);
        vector<vector<string>> expected = {{"5", "baz"}, {"5", "qux"}, {"5", "quux"}, {"5", "haha"}};
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }
}