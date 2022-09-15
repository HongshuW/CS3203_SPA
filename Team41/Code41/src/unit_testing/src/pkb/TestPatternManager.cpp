//
// Created by hongshu wang on 13/9/22.
//

#include "catch.hpp"

using namespace std;

TEST_CASE("Test Pattern Manager") {
//    PatternTable * patternTable = PatternManager::getPatterns();
//    *patternTable = PatternTable();
//
//    // pattern 1: x = x + 1;
//    vector<string> p1Metainfo{"1", "x"};
//    shared_ptr<ExprNode> p1Left = make_shared<ExprNode>(ExprNode("x"));
//    shared_ptr<ExprNode> p1Right = make_shared<ExprNode>(ExprNode("1"));
//    shared_ptr<ExprNode> pattern1 = make_shared<ExprNode>(ExprNode("+"));
//    pattern1->left = p1Left;
//    pattern1->right = p1Right;
//    PatternManager::savePattern(p1Metainfo, pattern1);
//
//    // pattern 2: y = y + (x + 1);
//    vector<string> p2Metainfo{"2", "y"};
//    shared_ptr<ExprNode> p2Left = make_shared<ExprNode>(ExprNode("y"));
//    shared_ptr<ExprNode> p2RLeft = make_shared<ExprNode>(ExprNode("x"));
//    shared_ptr<ExprNode> p2RRight = make_shared<ExprNode>(ExprNode("1"));
//    shared_ptr<ExprNode> p2Right = make_shared<ExprNode>(ExprNode("+"));
//    p2Right->left = p2RLeft;
//    p2Right->right = p2RRight;
//    shared_ptr<ExprNode> pattern2 = make_shared<ExprNode>(ExprNode("+"));
//    pattern2->left = p2Left;
//    pattern2->right = p2Right;
//    PatternManager::savePattern(p2Metainfo, pattern2);
//
//    // a pattern that is never added
//    shared_ptr<ExprNode> mismatchedPattern = make_shared<ExprNode>(ExprNode("a"));
//
//    SECTION ("Test FULL_MATCH: pattern1 (positive)") {
//        ExpressionSpec fullMatch = ExpressionSpec(ExpressionSpecType::FULL_MATCH, pattern1);
//        shared_ptr<Table> fullMatchTable = PatternManager::getMatchedPatterns(fullMatch);
//        REQUIRE(fullMatchTable->rows.size() == 1);
//        REQUIRE(fullMatchTable->rows[0][0] == "1");
//        REQUIRE(fullMatchTable->rows[0][1] == "x");
//    }
//
//    SECTION ("Test FULL_MATCH: mismatchedPattern (negative)") {
//        ExpressionSpec fullMatch = ExpressionSpec(ExpressionSpecType::FULL_MATCH, mismatchedPattern);
//        shared_ptr<Table> fullMatchTable = PatternManager::getMatchedPatterns(fullMatch);
//        REQUIRE(fullMatchTable->rows.empty());
//    }
//
//    SECTION ("Test PARTIAL_MATCH: pattern1 (positive)") {
//        ExpressionSpec partialMatch = ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, pattern1);
//        shared_ptr<Table> partialMatchTable = PatternManager::getMatchedPatterns(partialMatch);
//        REQUIRE(partialMatchTable->rows.size() == 2);
//        REQUIRE(partialMatchTable->rows[0][0] == "1");
//        REQUIRE(partialMatchTable->rows[0][1] == "x");
//        REQUIRE(partialMatchTable->rows[1][0] == "2");
//        REQUIRE(partialMatchTable->rows[1][1] == "y");
//    }
//
//    SECTION ("Test PARTIAL_MATCH: mismatchedPattern (negative)") {
//        ExpressionSpec partialMatch = ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, mismatchedPattern);
//        shared_ptr<Table> partialMatchTable = PatternManager::getMatchedPatterns(partialMatch);
//        REQUIRE(partialMatchTable->rows.empty());
//    }
//
//    SECTION ("Test wildcard (positive)") {
//        ExpressionSpec anyMatch = ExpressionSpec(ExpressionSpecType::ANY_MATCH);
//        shared_ptr<Table> anyMatchTable = PatternManager::getMatchedPatterns(anyMatch);
//        REQUIRE(anyMatchTable->rows.size() == 2);
//        REQUIRE(anyMatchTable->rows[0][0] == "1");
//        REQUIRE(anyMatchTable->rows[0][1] == "x");
//        REQUIRE(anyMatchTable->rows[1][0] == "2");
//        REQUIRE(anyMatchTable->rows[1][1] == "y");
//    }
}
