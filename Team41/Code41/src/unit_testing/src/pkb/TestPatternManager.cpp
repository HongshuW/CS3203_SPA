//
// Created by hongshu wang on 13/9/22.
//

#include "catch.hpp"
#include "pkb/PatternManager.h"

using namespace std;

TEST_CASE("Test Pattern Manager") {
    SECTION ("Save pattern") {
        vector<string> metainfo{"1", "x"};
        shared_ptr<ExprNode> left = make_shared<ExprNode>(ExprNode("x"));
        shared_ptr<ExprNode> right = make_shared<ExprNode>(ExprNode("1"));
        shared_ptr<ExprNode> pattern = make_shared<ExprNode>(ExprNode("+"));
        pattern->left = left;
        pattern->right = right;
        PatternManager::savePattern(metainfo, pattern);

        shared_ptr<ExprNode> mismatchedPattern = make_shared<ExprNode>(ExprNode("y"));

        // test FULL_MATCH
        ExpressionSpec fullMatch = ExpressionSpec(ExpressionSpecType::FULL_MATCH, pattern);
        ExpressionSpec fullMatchLeft = ExpressionSpec(ExpressionSpecType::FULL_MATCH, left);
        shared_ptr<Table> fullMatchTable = PatternManager::getMatchedPatterns(fullMatch);
        shared_ptr<Table> fullMatchLeftTable = PatternManager::getMatchedPatterns(fullMatchLeft);
        REQUIRE(fullMatchTable->rows[0][0] == "1");
        REQUIRE(fullMatchTable->rows[0][1] == "x");
        REQUIRE(fullMatchLeftTable->rows.empty());

        // test PARTIAL_MATCH
        ExpressionSpec partialMatchLeft = ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, left);
        ExpressionSpec partialMatchRight = ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, right);
        ExpressionSpec partialMatch = ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, pattern);
        ExpressionSpec partialMismatch = ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, mismatchedPattern);
        shared_ptr<Table> partialMatchLeftTable = PatternManager::getMatchedPatterns(partialMatchLeft);
        shared_ptr<Table> partialMatchRightTable = PatternManager::getMatchedPatterns(partialMatchRight);
        shared_ptr<Table> partialMatchTable = PatternManager::getMatchedPatterns(partialMatch);
        shared_ptr<Table> partialMismatchTable = PatternManager::getMatchedPatterns(partialMismatch);
        REQUIRE(partialMatchLeftTable->rows[0][0] == "1");
        REQUIRE(partialMatchLeftTable->rows[0][1] == "x");
        REQUIRE(partialMatchRightTable->rows[0][0] == "1");
        REQUIRE(partialMatchRightTable->rows[0][1] == "x");
        REQUIRE(partialMatchTable->rows[0][0] == "1");
        REQUIRE(partialMatchTable->rows[0][1] == "x");
        REQUIRE(partialMismatchTable->rows.empty());

        // test wildcard
        ExpressionSpec anyMatch = ExpressionSpec(ExpressionSpecType::ANY_MATCH);
        shared_ptr<Table> anyMatchTable = PatternManager::getMatchedPatterns(anyMatch);
        REQUIRE(anyMatchTable->rows[0][0] == "1");
        REQUIRE(anyMatchTable->rows[0][1] == "x");
    }
}
