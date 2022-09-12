//
// Created by hongshu wang on 2/9/22.
//

#include "AST/ExprNode.h"
#include "catch.hpp"
#include "pkb/DataModifier.h"
#include "pkb/EntityManager.h"
#include "pkb/PatternManager.h"
#include "pkb/RelationshipManager.h"
#include "query_builder/commons/ExpressionSpec.h"

using namespace std;

TEST_CASE("Test Data Modifier") {
    SECTION ("Save variables") {
        list<string> variables{"dummyVarX", "dummyVarY", "dummyVarZ"};
        int initialSize = (*EntityManager::getVariables()).rows.size();

        DataModifier dm;
        dm.saveVariables(variables);

        // check header is set automatically
        auto expected = *EntityManager::getVariables();
        REQUIRE((*EntityManager::getVariables()).header[0] == "$variable_name");

        // check variables are added
        REQUIRE((*EntityManager::getVariables()).rows[initialSize][0] == "dummyVarX");
        REQUIRE((*EntityManager::getVariables()).rows[initialSize + 1][0] == "dummyVarY");
        REQUIRE((*EntityManager::getVariables()).rows[initialSize + 2][0] == "dummyVarZ");
    }

    SECTION ("Save statements") {
        list<vector<string>> statements;
        vector<string> s1{"3", "assign"};
        vector<string> s2{"4", "if"};
        statements.push_back(s1);
        statements.push_back(s2);
        int initialSize = (*EntityManager::getStatements()).rows.size();

        DataModifier dm;
        dm.saveStatements(statements);

        // check header is set automatically
        REQUIRE((*EntityManager::getStatements()).header[0] == "$statement_number");
        REQUIRE((*EntityManager::getStatements()).header[1] == "$statement_type");

        // check statements are added
        REQUIRE((*EntityManager::getStatements()).rows[initialSize][0] == "3");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize][1] == "assign");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize + 1][0] == "4");
        REQUIRE((*EntityManager::getStatements()).rows[initialSize + 1][1] == "if");
    }

    SECTION ("Save parentT") {
        int initialSize = (*RelationshipManager::getParentT()).rows.size();
        DataModifier dm;
        dm.saveParentT(vector<string>{"1", "2"});
        dm.saveParentT(vector<string>{"1", "3"});
        dm.saveParentT(vector<string>{"2", "3"});

        // check header is set automatically
        REQUIRE((*RelationshipManager::getParentT()).header[0] == "$parent_statement");
        REQUIRE((*RelationshipManager::getParentT()).header[1] == "$child_statement");

        // check relationships are added
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize][0] == "1");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize][1] == "2");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 1][0] == "1");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 1][1] == "3");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 2][0] == "2");
        REQUIRE((*RelationshipManager::getParentT()).rows[initialSize + 2][1] == "3");
    }

    SECTION ("Save pattern") {
        DataModifier dm;
        vector<string> metainfo{"1", "x"};
        shared_ptr<ExprNode> left = make_shared<ExprNode>(ExprNode("x"));
        shared_ptr<ExprNode> right = make_shared<ExprNode>(ExprNode("1"));
        shared_ptr<ExprNode> pattern = make_shared<ExprNode>(ExprNode("+"));
        pattern->left = left;
        pattern->right = right;
        dm.savePattern(metainfo, pattern);

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