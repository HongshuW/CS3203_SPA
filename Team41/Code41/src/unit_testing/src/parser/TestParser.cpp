//
// Created by Xingchen Lin on 9/9/22.
//

#include "catch.hpp"

#include "parser/Parser.h"
#include "parser/SPExceptions.h"

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

        auto assignNode = make_shared<AssignNode>(
                make_shared<VariableNode>("x"),
                exprNode);
        vector<shared_ptr<StmtNode>> stmtList = vector<shared_ptr<StmtNode>>({assignNode});
        auto procedure = make_shared<ProcedureNode>("testPr0cedur3", stmtList);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 read statement") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "t12345", "{", "read", "num1", ";", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        vector<shared_ptr<StmtNode>> stmtList = vector<shared_ptr<StmtNode>>({readNode});
        auto procedure = make_shared<ProcedureNode>("t12345", stmtList);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 while statement") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "while", "(", "number", ">", "0", ")", "{",
                 "read", "num1", ";", "}", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        auto relExprNode =
                make_shared<RelExprNode>(make_shared<ExprNode>("number"),
                        ">",
                        make_shared<ExprNode>("0"));
        auto condExpr = make_shared<CondExprNode>(relExprNode);
        auto stmtListInner = vector<shared_ptr<StmtNode>>({readNode});
        auto whileNode = make_shared<WhileNode>(condExpr, stmtListInner);
        auto stmtListOuter = vector<shared_ptr<StmtNode>>({whileNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 while statement with complex condition") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "while", "(", "!", "(", "number", ">", "0", ")", ")", "{",
                 "read", "num1", ";", "}", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        auto relExprNode =
                make_shared<RelExprNode>(make_shared<ExprNode>("number"),
                                         ">",
                                         make_shared<ExprNode>("0"));
        auto condExpr = make_shared<CondExprNode>(make_shared<CondExprNode>(relExprNode));
        auto stmtListInner = vector<shared_ptr<StmtNode>>({readNode});
        auto whileNode = make_shared<WhileNode>(condExpr, stmtListInner);
        auto stmtListOuter = vector<shared_ptr<StmtNode>>({whileNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 if statement") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "number", ">", "0", ")", "then", "{",
                 "read", "num1", ";", "}", "else", "{", "read", "num2", ";", "}", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        auto readNodeElse = make_shared<ReadNode>(
                make_shared<VariableNode>("num2"));
        auto relExprNode =make_shared<RelExprNode>(make_shared<ExprNode>("number"),
                                 ">",
                                 make_shared<ExprNode>("0"));
        auto condExpr = make_shared<CondExprNode>(relExprNode);
        auto stmtListIf = vector<shared_ptr<StmtNode>>({readNode});
        auto stmtListElse = vector<shared_ptr<StmtNode>>({readNodeElse});
        auto ifNode = make_shared<IfNode>(condExpr, stmtListIf, stmtListElse);
        auto stmtListOuter = vector<shared_ptr<StmtNode>>({ifNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 if statement with complex condition") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "(", "x", "==", "y", ")", "&&", "(", "z", ">", "2", ")", ")", "then", "{",
                 "read", "num1", ";", "}", "else", "{", "read", "num2", ";", "}", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        auto readNodeElse = make_shared<ReadNode>(
                make_shared<VariableNode>("num2"));
        auto relExprNodeLHS =make_shared<RelExprNode>(make_shared<ExprNode>("x"),
                                                   "==",
                                                   make_shared<ExprNode>("y"));
        auto condExprLHS = make_shared<CondExprNode>(relExprNodeLHS);
        auto relExprNodeRHS =make_shared<RelExprNode>(make_shared<ExprNode>("z"),
                                                      ">",
                                                      make_shared<ExprNode>("2"));
        auto condExprRHS = make_shared<CondExprNode>(relExprNodeRHS);
        auto condExprRoot = make_shared<CondExprNode>(condExprLHS, "&&", condExprRHS);
        auto stmtListIf = vector<shared_ptr<StmtNode>>({readNode});
        auto stmtListElse = vector<shared_ptr<StmtNode>>({readNodeElse});
        auto ifNode = make_shared<IfNode>(condExprRoot, stmtListIf, stmtListElse);
        auto stmtListOuter = vector<shared_ptr<StmtNode>>({ifNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("Multiple levels of nesting") {
        auto tokens = std::vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "x", "<=", "100", ")", "then", "{",
                 "while", "(", "x", "!=", "100", ")", "{", "read", "num1", ";", "}", "}",
                 "else", "{", "print", "x", ";", "}", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        auto readNode = make_shared<ReadNode>(make_shared<VariableNode>("num1"));
        auto stmtListInner = vector<shared_ptr<StmtNode>>({readNode});
        auto whileRelExprNode =make_shared<RelExprNode>(make_shared<ExprNode>("x"),
                                                     "!=",
                                                     make_shared<ExprNode>("100"));
        auto whileCondExpr = make_shared<CondExprNode>(whileRelExprNode);
        auto printNode = make_shared<PrintNode>(
                make_shared<VariableNode>("x"));
        auto whileNode = make_shared<WhileNode>(whileCondExpr, stmtListInner);
        auto ifRelExprNode =make_shared<RelExprNode>(make_shared<ExprNode>("x"),
                                                      "<=",
                                                      make_shared<ExprNode>("100"));
        auto ifCondExpr = make_shared<CondExprNode>(ifRelExprNode);
        auto stmtListIf = vector<shared_ptr<StmtNode>>({whileNode});
        auto stmtListElse = vector<shared_ptr<StmtNode>>({printNode});
        auto ifNode = make_shared<IfNode>(ifCondExpr, stmtListIf, stmtListElse);
        auto stmtListOuter = vector<shared_ptr<StmtNode>>({ifNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtListOuter);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 while statement with complex conditional expression") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "while", "(", "(", "a", "<=", "z", "-", "(", "b", "-", "a", ")",
                 ")", "||", "(", "(", "!", "(", "xyz", "==", "123", ")", ")", "||", "(", "test", "<=", "3", "+", "90", ")", ")", ")",
                 "{", "read", "num1", ";", "}", "}"});
        Parser parser = Parser(tokens);
        REQUIRE_NOTHROW(parser.parse());
    }

    SECTION("Empty program, throw SPParserException") {
        vector<std::string> tokens = vector<std::string>(
                {});
        Parser parser = Parser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("Empty procedure, throw SPParserException") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "}"});
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
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "number", ">", "0", ")", "{", "read", "num1", ";","}", "else", "{", "read", "num2", ";", "}", "}"});
        Parser parser = Parser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("Empty statement in if loop, throw SPParserException") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "number", ">", "0", ")", "then", "{", "}", "else", "{", "read", "num2", ";", "}", "}"});
        Parser parser = Parser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("Empty statement in else loop, throw SPParserException") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "if", "(", "number", ">", "0", ")", "then", "{", "read", "num2",  ";", "}", "else", "{", "}", "}"});
        Parser parser = Parser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }

    SECTION("Empty statement in while loop, throw SPParserException") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "while", "(", "number", ">", "0", ")", "{", "}", "}"});
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
                {"procedure", "testProcedure", "{", "while", "(", "!", "(", "(", "number", ">", "0", ")", ")", "{",
                 "read", "num1", ";", "}", "}"});
        Parser parser = Parser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }


    SECTION("Invalid synonym name, throw SPParserException") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "while", "(", "!", "(", "(", "number", ">", "0", ")", ")", "{",
                 "read", "12num1", ";", "}", "}"});
        Parser parser = Parser(tokens);
        REQUIRE_THROWS_AS(parser.parse(), SPParseException);
    }
}