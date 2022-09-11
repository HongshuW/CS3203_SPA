//
// Created by Xingchen Lin on 11/9/22.
//

#include "catch.hpp"

#include "parser/CondExprParser.h"
#include "parser/Exceptions.h"

using namespace SourceParser;
using namespace std;

//! Parser will check for valid parentheses as well
TEST_CASE("Test SP CondExprNode Parser") {
    SECTION("x == 1") {
        vector<string> tokens = vector<string>(
                {"x", "==", "1"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("x");
        auto exprNodeRHS = make_shared<ExprNode>("1");
        auto relExprNode = make_shared<RelExprNode>(exprNodeLHS, "==", exprNodeRHS);
        auto expectedRoot = make_shared<CondExprNode>(relExprNode);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("x != y") {
        vector<string> tokens = vector<string>(
                {"x", "!=", "y"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("x");
        auto exprNodeRHS = make_shared<ExprNode>("y");
        auto relExprNode = make_shared<RelExprNode>(exprNodeLHS, "!=", exprNodeRHS);
        auto expectedRoot = make_shared<CondExprNode>(relExprNode);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("x <= y") {
        vector<string> tokens = vector<string>(
                {"x", "<=", "y"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("x");
        auto exprNodeRHS = make_shared<ExprNode>("y");
        auto relExprNode = make_shared<RelExprNode>(exprNodeLHS, "<=", exprNodeRHS);
        auto expectedRoot = make_shared<CondExprNode>(relExprNode);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("!(x == y)") {
        vector<string> tokens = vector<string>(
                {"!", "(", "x", "==", "y", ")"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("x");
        auto exprNodeRHS = make_shared<ExprNode>("y");
        auto relExprNode = make_shared<RelExprNode>(exprNodeLHS, "==", exprNodeRHS);
        auto condExprNodeLHS = make_shared<CondExprNode>(relExprNode);
        auto expectedRoot = make_shared<CondExprNode>(condExprNodeLHS);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("!(x + 1 <= y)") {
        vector<string> tokens = vector<string>(
                {"!", "(", "x", "+", "1", "<=", "y", ")"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("+");
        exprNodeLHS->left = make_shared<ExprNode>("x");
        exprNodeLHS->right = make_shared<ExprNode>("1");
        auto exprNodeRHS = make_shared<ExprNode>("y");
        auto relExprNode = make_shared<RelExprNode>(exprNodeLHS, "<=", exprNodeRHS);
        auto condExprNodeLHS = make_shared<CondExprNode>(relExprNode);
        auto expectedRoot = make_shared<CondExprNode>(condExprNodeLHS);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("(x == y) && (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", ")", "&&", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("x");
        auto exprNodeRHS = make_shared<ExprNode>("y");
        auto relExprNode =
                make_shared<RelExprNode>(exprNodeLHS, "==", exprNodeRHS);
        auto condExprNodeLHS = make_shared<CondExprNode>(relExprNode);

        auto exprNodeLHS_2 = make_shared<ExprNode>("z");
        auto exprNodeRHS_2 = make_shared<ExprNode>("2");
        auto relExprNode_2 =
                make_shared<RelExprNode>(exprNodeLHS_2, ">", exprNodeRHS_2);
        auto condExprNodeRHS = make_shared<CondExprNode>(relExprNode_2);
        auto expectedRoot =
                make_shared<CondExprNode>(condExprNodeLHS, "&&", condExprNodeRHS);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("((x == y) || (x <= 3)) && (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "(", "x", "==", "y", ")", "||", "(", "x", "<=", "3", ")", ")", "&&", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        // x == y
        auto exprNodeLHS_1 = make_shared<ExprNode>("x");
        auto exprNodeRHS_1 = make_shared<ExprNode>("y");
        auto relExprNode_1 =
                make_shared<RelExprNode>(exprNodeLHS_1, "==", exprNodeRHS_1);
        auto condExprNodeLHS_1 = make_shared<CondExprNode>(relExprNode_1);

        // x <=3
        auto exprNodeLHS_2 = make_shared<ExprNode>("x");
        auto exprNodeRHS_2 = make_shared<ExprNode>("3");
        auto relExprNode_2 =
                make_shared<RelExprNode>(exprNodeLHS_2, "<=", exprNodeRHS_2);
        auto condExprNodeRHS_2 = make_shared<CondExprNode>(relExprNode_2);

        // z > 2
        auto exprNodeLHS_3 = make_shared<ExprNode>("z");
        auto exprNodeRHS_3 = make_shared<ExprNode>("2");
        auto relExprNode_3 =
                make_shared<RelExprNode>(exprNodeLHS_3, ">", exprNodeRHS_3);

        // (z > 2)
        auto condExprNodeRHS = make_shared<CondExprNode>(relExprNode_3);
        // ((x == y) || (x <= 3))
        auto condExprNodeLHS =
                make_shared<CondExprNode>(condExprNodeLHS_1, "||", condExprNodeRHS_2);

        auto expectedRoot =
                make_shared<CondExprNode>(condExprNodeLHS, "&&", condExprNodeRHS);
        REQUIRE(*root == *expectedRoot);
    }

    SECTION("(x + 1 == y) && (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "+", "1", "==", "y", ")", "&&", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        shared_ptr<CondExprNode> root = parser.parse();

        auto exprNodeLHS = make_shared<ExprNode>("+");
        exprNodeLHS->left = make_shared<ExprNode>("x");
        exprNodeLHS->right = make_shared<ExprNode>("1");
        auto exprNodeRHS = make_shared<ExprNode>("y");
        auto relExprNode =
                make_shared<RelExprNode>(exprNodeLHS, "==", exprNodeRHS);
        auto condExprNodeLHS = make_shared<CondExprNode>(relExprNode);

        auto exprNodeLHS_2 = make_shared<ExprNode>("z");
        auto exprNodeRHS_2 = make_shared<ExprNode>("2");
        auto relExprNode_2 =
                make_shared<RelExprNode>(exprNodeLHS_2, ">", exprNodeRHS_2);
        auto condExprNodeRHS = make_shared<CondExprNode>(relExprNode_2);
        auto expectedRoot =
                make_shared<CondExprNode>(condExprNodeLHS, "&&", condExprNodeRHS);
        REQUIRE(*root == *expectedRoot);
    }

    //! Test for invalid inputs
    SECTION("(x == y && (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", "&&", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("(x == y) $$ (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", ")", "$$", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("x $$ 1") {
        vector<string> tokens = vector<string>(
                {"x", "&&", "1"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("(x &= y) || (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "&=", "y", ")", "||", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("(x == y || (z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", "||", "(", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("(x == y) || (z ^& 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", "||", "(", "z", "^&", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("(x == y || z > 2)") {
        vector<string> tokens = vector<string>(
                {"(", "x", "==", "y", "||", "z", ">", "2", ")"});
        CondExprParser parser = CondExprParser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }
}