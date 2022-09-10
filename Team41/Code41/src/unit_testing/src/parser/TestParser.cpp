//
// Created by Xingchen Lin on 9/9/22.
//

#include "catch.hpp"
#include "iostream"

#include "parser/Parser.h"

using namespace SourceParser;
using namespace std;

TEST_CASE("Test SP Parser") {
    SECTION("1 assign statement") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "x", "=", "x", "+", "1", ";", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        // simple program node
        auto assignNode = make_shared<AssignNode>(
                make_shared<VariableNode>("x"),
                make_shared<ExprNode>("x+1"));
        vector<shared_ptr<StmtNode>> stmtList = vector<shared_ptr<StmtNode>>({assignNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtList);
        vector<shared_ptr<ProcedureNode>> procedureList = vector<std::shared_ptr<ProcedureNode>>(
                {procedure});
        shared_ptr<ASTNode> expectedProgram = make_shared<ProgramNode>(procedureList);
        REQUIRE(*testProgram == *expectedProgram);
    }

    SECTION("1 read statement") {
        vector<std::string> tokens = vector<std::string>(
                {"procedure", "testProcedure", "{", "read", "num1", ";", "}"});
        Parser parser = Parser(tokens);
        shared_ptr<ASTNode> testProgram = parser.parse();

        // simple program node
        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        vector<shared_ptr<StmtNode>> stmtList = vector<shared_ptr<StmtNode>>({readNode});
        auto procedure = make_shared<ProcedureNode>("testProcedure", stmtList);
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

        // simple program node
        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        auto condExpr = make_shared<CondExprNode>("number>0");
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

        // simple program node
        auto readNode = make_shared<ReadNode>(
                make_shared<VariableNode>("num1"));
        auto readNodeElse = make_shared<ReadNode>(
                make_shared<VariableNode>("num2"));
        auto condExpr = make_shared<CondExprNode>("number>0");
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
}