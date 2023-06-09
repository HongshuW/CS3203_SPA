//
// Created by Xingchen Lin on 9/9/22.
//

#include "catch.hpp"
#include "parser/Parser.h"
#include "parser/SPExceptions.h"
#include "parser/Tokenizer.h"

using namespace SourceParser;
using namespace std;

TEST_CASE("Test SP Parser") {
  SECTION("1 assign statement") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testPr0cedur3", "{", "x", "=", "x", "+", "1", ";", "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto exprNode = make_shared<ExprNode>("+");
    exprNode->left = make_shared<ExprNode>("x");
    exprNode->right = make_shared<ExprNode>("1");

    auto assignNode =
        make_shared<AssignNode>(make_shared<VariableNode>("x"), exprNode);
    vector<shared_ptr<StmtNode>> stmtList =
        vector<shared_ptr<StmtNode>>({assignNode});
    auto procedure = make_shared<ProcedureNode>("testPr0cedur3", stmtList);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 read statement") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "t12345", "{", "read", "num1", ";", "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
    vector<shared_ptr<StmtNode>> stmtList =
        vector<shared_ptr<StmtNode>>({readNode});
    auto procedure = make_shared<ProcedureNode>("t12345", stmtList);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 while statement") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "while", "(", "number", ">", "0",
         ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
    auto relExprNode = make_shared<RelExprNode>(
        make_shared<ExprNode>("number"), ">", make_shared<ExprNode>("0"));
    auto condExpr = make_shared<CondExprNode>(relExprNode);
    auto stmtListInner = vector<shared_ptr<StmtNode>>({readNode});
    auto whileNode = make_shared<WhileNode>(condExpr, stmtListInner);
    auto stmtListOuter = vector<shared_ptr<StmtNode>>({whileNode});
    auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 while statement with complex condition") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "while", "(", "!", "(", "number",
         ">", "0", ")", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
    auto relExprNode = make_shared<RelExprNode>(
        make_shared<ExprNode>("number"), ">", make_shared<ExprNode>("0"));
    auto condExpr =
        make_shared<CondExprNode>(make_shared<CondExprNode>(relExprNode));
    auto stmtListInner = vector<shared_ptr<StmtNode>>({readNode});
    auto whileNode = make_shared<WhileNode>(condExpr, stmtListInner);
    auto stmtListOuter = vector<shared_ptr<StmtNode>>({whileNode});
    auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 if statement") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "if",
                             "(",         "number",
                             ">",         "0",
                             ")",         "then",
                             "{",         "read",
                             "num1",      ";",
                             "}",         "else",
                             "{",         "read",
                             "num2",      ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
    auto readNodeElse =
        make_shared<ReadNode>(make_shared<VariableNode>("num2"));
    auto relExprNode = make_shared<RelExprNode>(
        make_shared<ExprNode>("number"), ">", make_shared<ExprNode>("0"));
    auto condExpr = make_shared<CondExprNode>(relExprNode);
    auto stmtListIf = vector<shared_ptr<StmtNode>>({readNode});
    auto stmtListElse = vector<shared_ptr<StmtNode>>({readNodeElse});
    auto ifNode = make_shared<IfNode>(condExpr, stmtListIf, stmtListElse);
    auto stmtListOuter = vector<shared_ptr<StmtNode>>({ifNode});
    auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 if statement") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "if",
                             "(",         "number",
                             ">",         "0",
                             ")",         "then",
                             "{",         "read",
                             "num1",      ";",
                             "}",         "else",
                             "{",         "read",
                             "num2",      ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
    auto readNodeElse =
        make_shared<ReadNode>(make_shared<VariableNode>("num2"));
    auto relExprNode = make_shared<RelExprNode>(
        make_shared<ExprNode>("number"), ">", make_shared<ExprNode>("0"));
    auto condExpr = make_shared<CondExprNode>(relExprNode);
    auto stmtListIf = vector<shared_ptr<StmtNode>>({readNode});
    auto stmtListElse = vector<shared_ptr<StmtNode>>({readNodeElse});
    auto ifNode = make_shared<IfNode>(condExpr, stmtListIf, stmtListElse);
    auto stmtListOuter = vector<shared_ptr<StmtNode>>({ifNode});
    auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 if statement with complex condition") {
    std::string source =
        "procedure testProcedure {"
        "if ((x1 <= 1 - (x3 - a4)) || ((!(x4 == 2)) && (x2 != x3 + 1234))) "
        "then {"
        "x = 1;"
        "} else {"
        "x = 5;"
        "}"
        "}";

    Tokenizer tokenizer = Tokenizer(source);
    auto tokens = tokenizer.tokenize();
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("Multiple levels of nesting") {
    auto tokens = std::vector<std::string>({"procedure", "testProcedure",
                                            "{",         "if",
                                            "(",         "x",
                                            "<=",        "100",
                                            ")",         "then",
                                            "{",         "while",
                                            "(",         "x",
                                            "!=",        "100",
                                            "*",         "y",
                                            ")",         "{",
                                            "read",      "num1",
                                            ";",         "}",
                                            "}",         "else",
                                            "{",         "print",
                                            "x",         ";",
                                            "}",         "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
    auto stmtListInner = vector<shared_ptr<StmtNode>>({readNode});
    auto condExprNode = make_shared<ExprNode>("*");
    condExprNode->left = make_shared<ExprNode>("100");
    condExprNode->right = make_shared<ExprNode>("y");
    auto whileRelExprNode = make_shared<RelExprNode>(make_shared<ExprNode>("x"),
                                                     "!=", condExprNode);
    auto whileCondExpr = make_shared<CondExprNode>(whileRelExprNode);
    auto printNode = make_shared<PrintNode>(make_shared<VariableNode>("x"));
    auto whileNode = make_shared<WhileNode>(whileCondExpr, stmtListInner);
    auto ifRelExprNode = make_shared<RelExprNode>(
        make_shared<ExprNode>("x"), "<=", make_shared<ExprNode>("100"));
    auto ifCondExpr = make_shared<CondExprNode>(ifRelExprNode);
    auto stmtListIf = vector<shared_ptr<StmtNode>>({whileNode});
    auto stmtListElse = vector<shared_ptr<StmtNode>>({printNode});
    auto ifNode = make_shared<IfNode>(ifCondExpr, stmtListIf, stmtListElse);
    auto stmtListOuter = vector<shared_ptr<StmtNode>>({ifNode});
    auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("2 procedures") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "procedure1", "{", "call", "procedure2", ";", "}",
         "procedure", "procedure2", "{", "print", "num2", ";", "}"});
    Parser parser = Parser(tokens);
    shared_ptr<ASTNode> testProgram = parser.parse();

    auto callNode = make_shared<CallNode>("procedure2");
    vector<shared_ptr<StmtNode>> stmtList1 =
        vector<shared_ptr<StmtNode>>({callNode});
    auto procedure1 = make_shared<ProcedureNode>("procedure1", stmtList1);
    auto printNode = make_shared<PrintNode>(make_shared<VariableNode>("num2"));
    vector<shared_ptr<StmtNode>> stmtList2 =
        vector<shared_ptr<StmtNode>>({printNode});
    auto procedure2 = make_shared<ProcedureNode>("procedure2", stmtList2);
    vector<shared_ptr<ProcedureNode>> procedureList =
        vector<std::shared_ptr<ProcedureNode>>({procedure1, procedure2});
    shared_ptr<ASTNode> expectedProgram =
        make_shared<ProgramNode>(procedureList);
    REQUIRE(*testProgram == *expectedProgram);
  }

  SECTION("1 while statement with complex conditional expression") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "while",
                             "(",         "(",
                             "a",         "<=",
                             "z",         "-",
                             "(",         "b",
                             "-",         "a",
                             ")",         ")",
                             "||",        "(",
                             "(",         "!",
                             "(",         "xyz",
                             "==",        "123",
                             ")",         ")",
                             "||",        "(",
                             "test",      "<=",
                             "3",         "+",
                             "90",        ")",
                             ")",         ")",
                             "{",         "read",
                             "num1",      ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION("Empty program, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>({});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Empty procedure, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure", "{", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid procedure name, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "pr@c3dur3", "{", "read", "num1", ";", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid if syntax, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "if",
                             "(",         "number",
                             ">",         "0",
                             ")",         "{",
                             "read",      "num1",
                             ";",         "}",
                             "else",      "{",
                             "read",      "num2",
                             ";",         "}",
                             "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Empty statement in if loop, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "if", "(", "number", ">", "0", ")",
         "then", "{", "}", "else", "{", "read", "num2", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Empty statement in else loop, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "if", "(", "number", ">", "0", ")",
         "then", "{", "read", "num2", ";", "}", "else", "{", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Empty statement in while loop, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure", "{", "while", "(",
                             "number", ">", "0", ")", "{", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Missing ; token, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "read", "x", "=", "1", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid parentheses, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "while", "(", "!", "(", "(",
         "number", ">", "0", ")", ")", "{", "read", "num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid synonym name, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "while", "(", "!", "(", "(",
         "number", ">", "0", ")", ")", "{", "read", "12num1", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Negative number, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "-",
                             "99",        ";",
                             "while",     "(",
                             "x",         "<",
                             "-",         "100",
                             ")",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid number in assign statement, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "0123",
                             ";",         "while",
                             "(",         "x",
                             "<",         "100",
                             ")",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid name in assign statement, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "123x",
                             "=",         "123",
                             ";",         "while",
                             "(",         "x",
                             "<",         "100",
                             ")",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid number in assign condExpr, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "123",
                             ";",         "while",
                             "(",         "x",
                             "<",         "0123",
                             ")",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid name in assign condExpr, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "123",
                             ";",         "while",
                             "(",         "123x",
                             "<",         "123",
                             ")",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid bracket in if condition, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "123",
                             ";",         "if",
                             "{",         "x",
                             "<",         "123",
                             "}",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid bracket in while condition, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "123",
                             ";",         "while",
                             ";",         "x",
                             "<",         "123",
                             "}",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid variable name in read statement, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "read",
                             "123x",      ";",
                             "while",     "(",
                             "x",         "<",
                             "123",       ")",
                             "{",         "x",
                             "=",         "x",
                             "+",         "1",
                             ";",         "}",
                             "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid variable name in print statement, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "print",
                             "123x",      ";",
                             "while",     "(",
                             "x",         "<",
                             "123",       ")",
                             "{",         "x",
                             "=",         "x",
                             "+",         "1",
                             ";",         "}",
                             "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Empty expression in assign statement, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "testProcedure", "{", "print", "x", ";", "while", "(",
         "x", "<", "123", ")", "{", "x", "=", ";", "}", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid operator in while condition, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "print",
                             "x",         ";",
                             "while",     "(",
                             "x",         ">>",
                             "123",       ")",
                             "{",         "x",
                             "=",         "x",
                             "+",         "1",
                             ";",         "}",
                             "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid operator in if condition, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "123",
                             ";",         "if",
                             "(",         "x",
                             "123",       ")",
                             "{",         "x",
                             "=",         "x",
                             "+",         "1",
                             ";",         "}",
                             "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Empty condition in if condition, throw SPParserException") {
    vector<std::string> tokens =
        vector<std::string>({"procedure", "testProcedure",
                             "{",         "x",
                             "=",         "123",
                             ";",         "if",
                             "(",         "-",
                             ")",         "{",
                             "x",         "=",
                             "x",         "+",
                             "1",         ";",
                             "}",         "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }

  SECTION("Invalid name in call statement, throw SPParserException") {
    vector<std::string> tokens = vector<std::string>(
        {"procedure", "procedure1", "{", "call", "123test", ";", "}",
         "procedure", "procedure2", "{", "print", "num2", ";", "}"});
    Parser parser = Parser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), SPParseException);
  }
}