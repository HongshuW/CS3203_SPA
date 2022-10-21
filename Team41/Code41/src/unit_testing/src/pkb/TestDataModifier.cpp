//
// Created by hongshu wang on 2/9/22.
//

#include "AST/ExprNode.h"
#include "catch.hpp"
#include "pkb/DataModifier.h"

using namespace std;

TEST_CASE("Test Data Modifier") {
  shared_ptr<PKBStorage> pkbStorage = make_shared<PKBStorage>(PKBStorage());
  DataModifier dataModifier = DataModifier(pkbStorage);

  SECTION("Save variables") {
    list<string> variables{"dummyVarX", "dummyVarY", "dummyVarZ"};

    pkbStorage->getVariables()->dropRows();
    dataModifier.saveVariables(variables);
    shared_ptr<VariableTable> variableTable = pkbStorage->getVariables();

    // check header is set automatically
    REQUIRE(variableTable->header[0] == "$variable_name");

    // check variables are added
    REQUIRE(variableTable->rows[0][0] == "dummyVarX");
    REQUIRE(variableTable->rows[1][0] == "dummyVarY");
    REQUIRE(variableTable->rows[2][0] == "dummyVarZ");
  }

  SECTION("Save statements") {
    list<vector<string>> statements;
    vector<string> s1{"3", "assign"};
    vector<string> s2{"4", "if"};
    statements.push_back(s1);
    statements.push_back(s2);

    pkbStorage->getStatements()->dropRows();
    dataModifier.saveStatements(statements);
    shared_ptr<Table> statementTable = pkbStorage->getStatements();

    // check header is set automatically
    REQUIRE(statementTable->header[0] == "$statement_number");
    REQUIRE(statementTable->header[1] == "$statement_type");

    // check statements are added
    REQUIRE(statementTable->rows[0][0] == "3");
    REQUIRE(statementTable->rows[0][1] == "assign");
    REQUIRE(statementTable->rows[1][0] == "4");
    REQUIRE(statementTable->rows[1][1] == "if");
  }

  SECTION("Save parentT") {
    dataModifier.saveParentT(vector<string>{"1", "2"});
    dataModifier.saveParentT(vector<string>{"1", "3"});
    dataModifier.saveParentT(vector<string>{"2", "3"});
    shared_ptr<ParentTable> parentTTable = pkbStorage->getParentT();

    // check header is set automatically
    REQUIRE(parentTTable->header[0] == "$parent_statement");
    REQUIRE(parentTTable->header[1] == "$child_statement");

    // check relationships are added
    REQUIRE(parentTTable->rows[0][0] == "1");
    REQUIRE(parentTTable->rows[0][1] == "2");
    REQUIRE(parentTTable->rows[1][0] == "1");
    REQUIRE(parentTTable->rows[1][1] == "3");
    REQUIRE(parentTTable->rows[2][0] == "2");
    REQUIRE(parentTTable->rows[2][1] == "3");
  }

  SECTION("Save calls relation") {
    pkbStorage->getCalls()->dropRows();
    dataModifier.saveCalls(vector<string>{"proc1", "proc2", "5"});
    dataModifier.saveCalls(vector<string>{"proc2", "proc3", "12"});
    shared_ptr<Table> procedures = pkbStorage->getCalls();
    shared_ptr<Table> stmtNoProcMap = pkbStorage->getCallsProcedureNames();

    // check header is set automatically
    REQUIRE(procedures->header[0] == "$calling_procedure");
    REQUIRE(procedures->header[1] == "$called_procedure");
    REQUIRE(stmtNoProcMap->header[0] == "$statement_number");
    REQUIRE(stmtNoProcMap->header[1] == "$called_procedure");

    // check relationships are added
    REQUIRE(procedures->rows[0][0] == "proc1");
    REQUIRE(procedures->rows[0][1] == "proc2");
    REQUIRE(procedures->rows[1][0] == "proc2");
    REQUIRE(procedures->rows[1][1] == "proc3");
    REQUIRE(stmtNoProcMap->rows[0][0] == "5");
    REQUIRE(stmtNoProcMap->rows[0][1] == "proc2");
    REQUIRE(stmtNoProcMap->rows[1][0] == "12");
    REQUIRE(stmtNoProcMap->rows[1][1] == "proc3");
  }

  SECTION("Save assign pattern") {
    vector<string> metainfo{"1", "x"};
    shared_ptr<ExprNode> left = make_shared<ExprNode>(ExprNode("x"));
    shared_ptr<ExprNode> right = make_shared<ExprNode>(ExprNode("1"));
    shared_ptr<ExprNode> pattern = make_shared<ExprNode>(ExprNode("+"));
    pattern->left = left;
    pattern->right = right;
    dataModifier.saveAssignPattern(metainfo, pattern);
    shared_ptr<PatternTable> patternTable = pkbStorage->getAssignPatterns();

    // check header is set automatically
    REQUIRE(patternTable->header[0] == "$statement_number");
    REQUIRE(patternTable->header[1] == "$variable_name");

    // check pattern is added
    REQUIRE(patternTable->rows[0][0] == "1");
    REQUIRE(patternTable->rows[0][1] == "x");
    REQUIRE(patternTable->patternColumn[0] == pattern);
  }
}