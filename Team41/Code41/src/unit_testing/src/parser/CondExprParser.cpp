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