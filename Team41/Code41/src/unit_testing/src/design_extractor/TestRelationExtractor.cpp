//
// Created by hongshu wang on 10/9/22.
//

#include "catch.hpp"
#include "AST/PrintNode.h"
#include "AST/ReadNode.h"
#include "AST/IfNode.h"
#include "AST/WhileNode.h"
#include "AST/utils/ASTUtils.h"
#include <unordered_map>
#include "design_extractor/DesignExtractor.h"
#include "pkb/DataModifier.h"

using namespace std;
using namespace DE;

TEST_CASE("Test relation extractor") {
    SECTION("Parent: no relations") {
        /*
        * procedure CASE1 {
        * 1 x = 1
        * 2 y = 2
        * }
        */
        shared_ptr<AssignNode> assignX = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        shared_ptr<AssignNode> assignY = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("2"));
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("CASE1", {assignX, assignY}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        REQUIRE(actualParentRelations->empty());
    }

    SECTION("Parent: has relations") {
        /*
        * procedure CASE2 {
        * 1 x = 1
        * 2 while (x != 0) {
        * 3     x = 0;
        *   }
        * 4 y = 2
        * }
        */
        shared_ptr<AssignNode> assignX1 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        shared_ptr<AssignNode> assignX2 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("0"));
        vector<shared_ptr<StmtNode>> stmtLst{assignX2};
        shared_ptr<WhileNode> whileNode = make_shared<WhileNode>(make_shared<CondExprNode>("x != 0"), stmtLst);
        shared_ptr<AssignNode> assignY = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("2"));
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("CASE2", {assignX1, whileNode, assignX2, assignY}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<string> expectedR1 = {"2", "3"};
        list<vector<string>> expectedParentRelations{expectedR1};
        auto actualIterator = actualParentRelations->begin();
        auto expectedIterator = expectedParentRelations.begin();
        while (actualIterator != actualParentRelations->end() && expectedIterator != expectedParentRelations.end()) {
            vector<string> a = *actualIterator;
            vector<string> e = *expectedIterator;
            REQUIRE(a[0] == e[0]);
            REQUIRE(a[1] == e[1]);
            advance(actualIterator, 1);
            advance(expectedIterator, 1);
        }
        REQUIRE(actualIterator == actualParentRelations->end());
        REQUIRE(expectedIterator == expectedParentRelations.end());
    }
}