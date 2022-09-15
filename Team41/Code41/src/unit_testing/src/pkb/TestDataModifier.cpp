//
// Created by hongshu wang on 2/9/22.
//

#include "AST/ExprNode.h"
#include "catch.hpp"
#include "pkb/DataModifier.h"

using namespace std;

TEST_CASE("Test Data Modifier") {
//    SECTION ("Save variables") {
//        list<string> variables{"dummyVarX", "dummyVarY", "dummyVarZ"};
//        int initialSize = (*EntityManager::getVariables()).rows.size();
//
//        DataModifier dm;
//        dm.saveVariables(variables);
//        VariableTable variableTable = *EntityManager::getVariables();
//
//        // check header is set automatically
//        auto expected = *EntityManager::getVariables();
//        REQUIRE(variableTable.header[0] == "$variable_name");
//
//        // check variables are added
//        REQUIRE(variableTable.rows[initialSize][0] == "dummyVarX");
//        REQUIRE(variableTable.rows[initialSize + 1][0] == "dummyVarY");
//        REQUIRE(variableTable.rows[initialSize + 2][0] == "dummyVarZ");
//    }
//
//    SECTION ("Save statements") {
//        list<vector<string>> statements;
//        vector<string> s1{"3", "assign"};
//        vector<string> s2{"4", "if"};
//        statements.push_back(s1);
//        statements.push_back(s2);
//        int initialSize = (*EntityManager::getStatements()).rows.size();
//
//        DataModifier dm;
//        dm.saveStatements(statements);
//        StatementTable statementTable = *EntityManager::getStatements();
//
//        // check header is set automatically
//        REQUIRE(statementTable.header[0] == "$statement_number");
//        REQUIRE(statementTable.header[1] == "$statement_type");
//
//        // check statements are added
//        REQUIRE(statementTable.rows[initialSize][0] == "3");
//        REQUIRE(statementTable.rows[initialSize][1] == "assign");
//        REQUIRE(statementTable.rows[initialSize + 1][0] == "4");
//        REQUIRE(statementTable.rows[initialSize + 1][1] == "if");
//    }
//
//    SECTION ("Save parentT") {
//        int initialSize = (*RelationshipManager::getParentT()).rows.size();
//        DataModifier dm;
//        dm.saveParentT(vector<string>{"1", "2"});
//        dm.saveParentT(vector<string>{"1", "3"});
//        dm.saveParentT(vector<string>{"2", "3"});
//        ParentTable parentTTable = *RelationshipManager::getParentT();
//
//        // check header is set automatically
//        REQUIRE(parentTTable.header[0] == "$parent_statement");
//        REQUIRE(parentTTable.header[1] == "$child_statement");
//
//        // check relationships are added
//        REQUIRE(parentTTable.rows[initialSize][0] == "1");
//        REQUIRE(parentTTable.rows[initialSize][1] == "2");
//        REQUIRE(parentTTable.rows[initialSize + 1][0] == "1");
//        REQUIRE(parentTTable.rows[initialSize + 1][1] == "3");
//        REQUIRE(parentTTable.rows[initialSize + 2][0] == "2");
//        REQUIRE(parentTTable.rows[initialSize + 2][1] == "3");
//    }
//
//    SECTION ("Save pattern") {
//        int initialSize = (*PatternManager::getPatterns()).rows.size();
//        DataModifier dm;
//        vector<string> metainfo{"1", "x"};
//        shared_ptr<ExprNode> left = make_shared<ExprNode>(ExprNode("x"));
//        shared_ptr<ExprNode> right = make_shared<ExprNode>(ExprNode("1"));
//        shared_ptr<ExprNode> pattern = make_shared<ExprNode>(ExprNode("+"));
//        pattern->left = left;
//        pattern->right = right;
//        dm.savePattern(metainfo, pattern);
//        PatternTable patternTable = *PatternManager::getPatterns();
//
//        // check header is set automatically
//        REQUIRE(patternTable.header[0] == "$statement_number");
//        REQUIRE(patternTable.header[1] == "$variable_name");
//
//        // check pattern is added
//        REQUIRE(patternTable.rows[initialSize][0] == "1");
//        REQUIRE(patternTable.rows[initialSize][1] == "x");
//        REQUIRE(patternTable.patternColumn[0] == pattern);
//    }
}