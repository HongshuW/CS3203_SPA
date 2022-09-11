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
    SECTION("CASE1") {
        /*
        * procedure CASE1 {
        * 1 x = 1
        * 2 y = 2
        * }
        */
        // Build up AST
        shared_ptr<AssignNode> assignX = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        shared_ptr<AssignNode> assignY = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("2"));
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("CASE1", {assignX, assignY}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent(*)
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        REQUIRE(actualParentRelations->empty());
        REQUIRE(actualParentTRelations->empty());

        // Test ModifiesS
        shared_ptr<list<vector<string>>> actualModifiesSRelations = designExtractor.extractRelations(RelationType::MODIFIES_S);
        vector<string> expectedR1{"1", "x"};
        vector<string> expectedR2{"2", "y"};
        list<vector<string>> expectedModifiesSRelations{expectedR1, expectedR2};
        auto actualModifiesSIterator = actualModifiesSRelations->begin();
        auto expectedModifiesSIterator = expectedModifiesSRelations.begin();
        while (actualModifiesSIterator != actualModifiesSRelations->end()
        && expectedModifiesSIterator != expectedModifiesSRelations.end()) {
            vector<string> a = *actualModifiesSIterator;
            vector<string> e = *expectedModifiesSIterator;
            REQUIRE(a[0] == e[0]);
            REQUIRE(a[1] == e[1]);
            advance(actualModifiesSIterator, 1);
            advance(expectedModifiesSIterator, 1);
        }
        REQUIRE(actualModifiesSIterator == actualModifiesSRelations->end());
        REQUIRE(expectedModifiesSIterator == expectedModifiesSRelations.end());
    }

    SECTION("CASE2") {
        /*
        * procedure CASE2 {
        * 1 x = 1
        * 2 while (x != 0) {
        * 3     x = 0
        * 4     if (x == 0) then {
        * 5         y = 1
        *       } else {
        * 6         y = 0
        *       }
        *   }
        * 7 y = 2
        * }
        */

        // 1
        shared_ptr<AssignNode> assignX1 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("1"));
        // 3
        shared_ptr<AssignNode> assignX2 = make_shared<AssignNode>(make_shared<VariableNode>("x"), make_shared<ExprNode>("0"));
        // 5
        shared_ptr<AssignNode> assignY1 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("1"));
        // 6
        shared_ptr<AssignNode> assignY2 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("0"));
        // 7
        shared_ptr<AssignNode> assignY3 = make_shared<AssignNode>(make_shared<VariableNode>("y"), make_shared<ExprNode>("2"));
        // 4
        vector<shared_ptr<StmtNode>> ifStmtLst{assignY1};
        vector<shared_ptr<StmtNode>> elseStmtLst{assignY2};
        shared_ptr<IfNode> ifNode = make_shared<IfNode>(make_shared<CondExprNode>("x == 0"), ifStmtLst, elseStmtLst);
        // 2
        vector<shared_ptr<StmtNode>> whileStmtLst{assignX2, ifNode};
        shared_ptr<WhileNode> whileNode = make_shared<WhileNode>(make_shared<CondExprNode>("x != 0"), whileStmtLst);
        // procedure & program
        shared_ptr<ProcedureNode> procedureNode = make_shared<ProcedureNode>(ProcedureNode("CASE2", {assignX1, whileNode, assignY3}));
        shared_ptr<ProgramNode> programNode = make_shared<ProgramNode>(ProgramNode({procedureNode}));
        DataModifier dataModifier = DataModifier();
        DesignExtractor designExtractor = DesignExtractor(dataModifier, programNode);

        // Test Parent
        shared_ptr<list<vector<string>>> actualParentRelations = designExtractor.extractRelations(RelationType::PARENT);
        vector<string> expectedR1 = {"2", "3"};
        vector<string> expectedR2 = {"2", "4"};
        vector<string> expectedR3 = {"4", "5"};
        vector<string> expectedR4 = {"4", "6"};
        list<vector<string>> expectedParentRelations{expectedR1, expectedR2, expectedR3, expectedR4};
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

        // Test Parent*
        shared_ptr<list<vector<string>>> actualParentTRelations = designExtractor.extractRelations(RelationType::PARENT_T);
        vector<string> expectedR5 = {"2", "5"};
        vector<string> expectedR6 = {"2", "6"};
        list<vector<string>> expectedParentTRelations{expectedR1, expectedR2, expectedR5, expectedR6, expectedR3, expectedR4};
        auto actualParentTIterator = actualParentTRelations->begin();
        auto expectedParentTIterator = expectedParentTRelations.begin();
        while (actualParentTIterator != actualParentTRelations->end()
        && expectedParentTIterator != expectedParentTRelations.end()) {
            vector<string> a = *actualParentTIterator;
            vector<string> e = *expectedParentTIterator;
            REQUIRE(a[0] == e[0]);
            REQUIRE(a[1] == e[1]);
            advance(actualParentTIterator, 1);
            advance(expectedParentTIterator, 1);
        }
        REQUIRE(actualParentTIterator == actualParentTRelations->end());
        REQUIRE(expectedParentTIterator == expectedParentTRelations.end());

        // Test ModifiesS
        shared_ptr<list<vector<string>>> actualModifiesSRelations = designExtractor.extractRelations(RelationType::MODIFIES_S);
        vector<string> expectedModifiesSR1{"1", "x"};
        vector<string> expectedModifiesSR2{"2", "x"};
        vector<string> expectedModifiesSR3{"2", "y"};
        vector<string> expectedModifiesSR4{"3", "x"};
        vector<string> expectedModifiesSR5{"4", "y"};
        vector<string> expectedModifiesSR6{"5", "y"};
        vector<string> expectedModifiesSR7{"6", "y"};
        vector<string> expectedModifiesSR8{"7", "y"};
        list<vector<string>> expectedModifiesSRelations{expectedModifiesSR1, expectedModifiesSR2, expectedModifiesSR3,
                                                        expectedModifiesSR4, expectedModifiesSR5, expectedModifiesSR6,
                                                        expectedModifiesSR7, expectedModifiesSR8};
        auto actualModifiesSIterator = actualModifiesSRelations->begin();
        auto expectedModifiesSIterator = expectedModifiesSRelations.begin();
        while (actualModifiesSIterator != actualModifiesSRelations->end()
        && expectedModifiesSIterator != expectedModifiesSRelations.end()) {
            vector<string> a = *actualModifiesSIterator;
            vector<string> e = *expectedModifiesSIterator;
            REQUIRE(a[0] == e[0]);
            REQUIRE(a[1] == e[1]);
            advance(actualModifiesSIterator, 1);
            advance(expectedModifiesSIterator, 1);
        }
        REQUIRE(actualModifiesSIterator == actualModifiesSRelations->end());
        REQUIRE(expectedModifiesSIterator == expectedModifiesSRelations.end());
    }
}