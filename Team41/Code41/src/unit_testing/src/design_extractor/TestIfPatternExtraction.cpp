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

TEST_CASE("Test If Pattern Extractor") {
    SECTION("test empty procedure") {
        auto programNode = TestDE::Dummies::getTestProgramNode(0);
        list<vector<string>> actual = PatternExtractor::extractIfPattern(programNode);
        REQUIRE(actual.empty());
    }

    SECTION("test simple if statement") {
        auto programNode = TestDE::Dummies::getTestProgramNode(2);
        list<vector<string>> actual = PatternExtractor::extractIfPattern(programNode);
        vector<vector<string>> expected = {{"3", "bar"}, {"3", "y"}};
        REQUIRE(expected.size() == actual.size());
        REQUIRE(TestDE::DEUtils::containsSameElementPair(actual, expected));
    }
}