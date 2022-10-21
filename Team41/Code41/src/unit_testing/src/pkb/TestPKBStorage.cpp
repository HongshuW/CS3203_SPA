//
// Created by hongshu wang on 16/9/22.
//

#include "catch.hpp"
#include "pkb/PKBStorage.h"

using namespace std;

TEST_CASE("Test PKBStorage") {
  shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());

  SECTION("Save variables") {
    list<string> variables{"dummyVarA", "dummyVarB"};
    pkbStorage->getVariables()->dropRows();
    pkbStorage->saveVariables(variables);

    // check header is set automatically
    REQUIRE(pkbStorage->getVariables()->header[0] == "$variable_name");

    // check variables are added
    REQUIRE(pkbStorage->getVariables()->rows[0][0] == "dummyVarA");
    REQUIRE(pkbStorage->getVariables()->rows[1][0] == "dummyVarB");
  }

  SECTION("Save statements, UsesS, ModifiesS -> check statementTable") {
    // save statements
    list<vector<string>> statements;
    vector<string> s1{"1", "while"};
    vector<string> s2{"2", "read"};
    vector<string> s3{"3", "print"};
    statements.push_back(s1);
    statements.push_back(s2);
    statements.push_back(s3);
    pkbStorage->getStatements()->dropRows();
    pkbStorage->saveStatements(statements);
    shared_ptr<Table> stmtsTable = pkbStorage->getStatements();

    // check header is set automatically
    REQUIRE(stmtsTable->header[0] == "$statement_number");
    REQUIRE(stmtsTable->header[1] == "$statement_type");

    // check statements are added
    REQUIRE(stmtsTable->rows[0][0] == "1");
    REQUIRE(stmtsTable->rows[0][1] == "while");
    REQUIRE(stmtsTable->rows[1][0] == "2");
    REQUIRE(stmtsTable->rows[1][1] == "read");

    // save UsesS
    vector<string> u1 = vector<string>{"1", "used_in_while"};
    vector<string> u2 = vector<string>{"3", "printed"};
    pkbStorage->getPrintVariableNames()->dropRows();
    pkbStorage->saveUsesS(u1);
    pkbStorage->saveUsesS(u2);
    shared_ptr<Table> printedVars = pkbStorage->getPrintVariableNames();

    // check header is set automatically
    REQUIRE(printedVars->header[0] == "$statement_number");
    REQUIRE(printedVars->header[1] == "$variable_name");

    // check printed variables are added
    REQUIRE(printedVars->rows[0][0] == "3");
    REQUIRE(printedVars->rows[0][1] == "printed");

    // save ModifiesS
    vector<string> m1 = vector<string>{"1", "modified_in_while"};
    vector<string> m2 = vector<string>{"2", "read"};
    pkbStorage->getReadVariableNames()->dropRows();
    pkbStorage->saveModifiesS(m1);
    pkbStorage->saveModifiesS(m2);
    shared_ptr<Table> readVars = pkbStorage->getReadVariableNames();

    // check header is set automatically
    REQUIRE(readVars->header[0] == "$statement_number");
    REQUIRE(readVars->header[1] == "$variable_name");

    // check read variables are added
    REQUIRE(readVars->rows[0][0] == "2");
    REQUIRE(readVars->rows[0][1] == "read");
  }

  SECTION("Save parent") {
    pkbStorage->getParent()->dropRows();
    vector<string> s1{"1", "2"};
    vector<string> s2{"2", "3"};
    pkbStorage->saveParent(s1);
    pkbStorage->saveParent(s2);

    // check header is set automatically
    REQUIRE(pkbStorage->getParent()->header[0] == "$parent_statement");
    REQUIRE(pkbStorage->getParent()->header[1] == "$child_statement");

    // check relationships are added
    REQUIRE(pkbStorage->getParent()->rows[0][0] == "1");
    REQUIRE(pkbStorage->getParent()->rows[0][1] == "2");
    REQUIRE(pkbStorage->getParent()->rows[1][0] == "2");
    REQUIRE(pkbStorage->getParent()->rows[1][1] == "3");
  }

  SECTION("Save calls") {
    pkbStorage->getCalls()->dropRows();
    vector<string> s1{"p1", "p2", "1"};
    vector<string> s2{"p1", "p3", "2"};
    vector<string> s3{"p2", "p3", "10"};
    pkbStorage->saveCalls(s1);
    pkbStorage->saveCalls(s2);
    pkbStorage->saveCalls(s3);

    shared_ptr<Table> procedures = pkbStorage->getCalls();
    shared_ptr<Table> stmtNoProcMap = pkbStorage->getCallsProcedureNames();

    // check header is set automatically
    REQUIRE(procedures->header[0] == "$calling_procedure");
    REQUIRE(procedures->header[1] == "$called_procedure");
    REQUIRE(stmtNoProcMap->header[0] == "$statement_number");
    REQUIRE(stmtNoProcMap->header[1] == "$called_procedure");

    // check relationships are added
    REQUIRE(procedures->rows[0][0] == "p1");
    REQUIRE(procedures->rows[0][1] == "p2");
    REQUIRE(procedures->rows[1][0] == "p1");
    REQUIRE(procedures->rows[1][1] == "p3");
    REQUIRE(procedures->rows[2][0] == "p2");
    REQUIRE(procedures->rows[2][1] == "p3");
    REQUIRE(stmtNoProcMap->rows[0][0] == "1");
    REQUIRE(stmtNoProcMap->rows[0][1] == "p2");
    REQUIRE(stmtNoProcMap->rows[1][0] == "2");
    REQUIRE(stmtNoProcMap->rows[1][1] == "p3");
    REQUIRE(stmtNoProcMap->rows[2][0] == "10");
    REQUIRE(stmtNoProcMap->rows[2][1] == "p3");
  }

  // Testing of assign patterns

  shared_ptr<PatternTable> patternTable = pkbStorage->getAssignPatterns();
  *patternTable = PatternTable();

  // pattern 1: x = x + 1;
  vector<string> p1Metainfo{"1", "x"};
  shared_ptr<ExprNode> p1Left = make_shared<ExprNode>(ExprNode("x"));
  shared_ptr<ExprNode> p1Right = make_shared<ExprNode>(ExprNode("1"));
  shared_ptr<ExprNode> pattern1 = make_shared<ExprNode>(ExprNode("+"));
  pattern1->left = p1Left;
  pattern1->right = p1Right;
  pkbStorage->saveAssignPattern(p1Metainfo, pattern1);

  // pattern 2: y = y + (x + 1);
  vector<string> p2Metainfo{"2", "y"};
  shared_ptr<ExprNode> p2Left = make_shared<ExprNode>(ExprNode("y"));
  shared_ptr<ExprNode> p2RLeft = make_shared<ExprNode>(ExprNode("x"));
  shared_ptr<ExprNode> p2RRight = make_shared<ExprNode>(ExprNode("1"));
  shared_ptr<ExprNode> p2Right = make_shared<ExprNode>(ExprNode("+"));
  p2Right->left = p2RLeft;
  p2Right->right = p2RRight;
  shared_ptr<ExprNode> pattern2 = make_shared<ExprNode>(ExprNode("+"));
  pattern2->left = p2Left;
  pattern2->right = p2Right;
  pkbStorage->saveAssignPattern(p2Metainfo, pattern2);

  // a pattern that is never added
  shared_ptr<ExprNode> mismatchedPattern = make_shared<ExprNode>(ExprNode("a"));

  SECTION("Test FULL_MATCH: pattern1 (positive)") {
    ExpressionSpec fullMatch =
        ExpressionSpec(ExpressionSpecType::FULL_MATCH, pattern1);
    shared_ptr<Table> fullMatchTable =
        pkbStorage->getMatchedAssignPatterns(fullMatch);
    REQUIRE(fullMatchTable->rows.size() == 1);
    REQUIRE(fullMatchTable->rows[0][0] == "1");
    REQUIRE(fullMatchTable->rows[0][1] == "x");
  }

  SECTION("Test FULL_MATCH: mismatchedPattern (negative)") {
    ExpressionSpec fullMatch =
        ExpressionSpec(ExpressionSpecType::FULL_MATCH, mismatchedPattern);
    shared_ptr<Table> fullMatchTable =
        pkbStorage->getMatchedAssignPatterns(fullMatch);
    REQUIRE(fullMatchTable->rows.empty());
  }

  SECTION("Test PARTIAL_MATCH: pattern1 (positive)") {
    ExpressionSpec partialMatch =
        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, pattern1);
    shared_ptr<Table> partialMatchTable =
        pkbStorage->getMatchedAssignPatterns(partialMatch);
    REQUIRE(partialMatchTable->rows.size() == 2);
    REQUIRE(partialMatchTable->rows[0][0] == "1");
    REQUIRE(partialMatchTable->rows[0][1] == "x");
    REQUIRE(partialMatchTable->rows[1][0] == "2");
    REQUIRE(partialMatchTable->rows[1][1] == "y");
  }

  SECTION("Test PARTIAL_MATCH: mismatchedPattern (negative)") {
    ExpressionSpec partialMatch =
        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, mismatchedPattern);
    shared_ptr<Table> partialMatchTable =
        pkbStorage->getMatchedAssignPatterns(partialMatch);
    REQUIRE(partialMatchTable->rows.empty());
  }

  SECTION("Test wildcard (positive)") {
    ExpressionSpec anyMatch = ExpressionSpec(ExpressionSpecType::ANY_MATCH);
    shared_ptr<Table> anyMatchTable =
        pkbStorage->getMatchedAssignPatterns(anyMatch);
    REQUIRE(anyMatchTable->rows.size() == 2);
    REQUIRE(anyMatchTable->rows[0][0] == "1");
    REQUIRE(anyMatchTable->rows[0][1] == "x");
    REQUIRE(anyMatchTable->rows[1][0] == "2");
    REQUIRE(anyMatchTable->rows[1][1] == "y");
  }
}