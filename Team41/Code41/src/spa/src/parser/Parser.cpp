//
// Created by Tan Wei En on 2/9/22.
//

#include "Parser.h"
#include "Exceptions.h"
#include "iostream"

using namespace SourceParser;
using namespace std;

// consider creating Token class instead of string for tokens.

Parser::Parser(vector<string> tokens)
    : currIdx(0), tokens(tokens) {};

string Parser::peek() { return tokens[currIdx]; }

string Parser::previous() { return tokens[currIdx - 1]; }

string Parser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool Parser::match(string s) {
    if (peek().compare(s) == 0) {
        currIdx++;
        return true;
    }
    return false;
}

bool Parser::expect(string s) {
    if (match(s)) {
        return true;
    }
    throw SPParseException("Expect '" + s + "', got '" + peek() + "'.");
}

shared_ptr<ProcedureNode> Parser::parseProcedureNode() {
    expect("procedure");
    //! need to check for valid procedureName
    string procedureName = pop();
    if (!Utils::isValidName(procedureName)) {
        throw SPParseException("Invalid procedure name: " + procedureName + ".");
    }
    expect("{");
    vector<shared_ptr<StmtNode>> statementList = parseStatementList();
    expect("}");

    return make_shared<ProcedureNode>(procedureName, statementList);
}

vector<shared_ptr<StmtNode>> Parser::parseStatementList() {
    vector<shared_ptr<StmtNode>> statementList;
    while (true) {
        shared_ptr<StmtNode> statementNode = parseStatementNode();
        if (statementNode) {
            statementList.push_back(statementNode);
        } else {
            break;
        }
    }

    if (statementList.size() == 0) {
        throw SPParseException("Statement list cannot be empty.");
    }
    return statementList;
}

shared_ptr<StmtNode> Parser::parseStatementNode() {
    //! if, read, assign, print, while, call
    if (peek().compare("}") == 0) {
        // end of stmtList
        return nullptr;
    }

    int savedIdx = currIdx;

    shared_ptr<StmtNode> ifStatementNode = parseIfNode();
    if (ifStatementNode) return ifStatementNode;

    shared_ptr<StmtNode> readStatementNode = parseReadNode();
    if (readStatementNode) return readStatementNode;

    shared_ptr<StmtNode> printStatementNode = parsePrintNode();
    if (printStatementNode) return printStatementNode;

    shared_ptr<StmtNode> whileStatementNode = parseWhileNode();
    if (whileStatementNode) return whileStatementNode;

    shared_ptr<StmtNode> callStatementNode = parseCallNode();
    if (callStatementNode) return callStatementNode;

    shared_ptr<StmtNode> assignStatementNode = parseAssignNode();
    if (assignStatementNode) return assignStatementNode;

    //! none of the stmtNode can be parsed, we revert the currIdx
    currIdx = savedIdx;
    return nullptr;
}

shared_ptr<IfNode> Parser::parseIfNode() {
    int savedIdx = currIdx;

    //! if may be a variable name
    if (!match("if")) {
        currIdx = savedIdx;
        return nullptr;
    }

    if (!match("(")) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<CondExprNode> condExprNode = parseCondExprNode();

    if (!condExprNode) {
        throw SPParseException("Expect a conditional expression, got " + peek());
    }

    expect(")");
    expect("then");
    expect("{");
    vector<shared_ptr<StmtNode>> ifStatementList = parseStatementList();
    expect("}");
    expect("else");
    expect("{");
    vector<shared_ptr<StmtNode>> elseStatementList = parseStatementList();
    expect("}");
    shared_ptr<IfNode> ifNode = make_shared<IfNode>(condExprNode, ifStatementList, elseStatementList);
    return ifNode;
}

shared_ptr<ReadNode> Parser::parseReadNode() {
    int savedIdx = currIdx;
    if (!match("read")) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<VariableNode> varNode = parseVariableNode();

    if (!varNode) {
        currIdx = savedIdx;
        return nullptr;
    }
    //! To check for syntax error
    expect(";");
    shared_ptr<ReadNode> readNode = make_shared<ReadNode>(varNode);
    return readNode;
}

shared_ptr<AssignNode> Parser::parseAssignNode() {
    shared_ptr<VariableNode> variableNode = parseVariableNode();

    if (!variableNode) {
        throw SPParseException("Expect a variable name for assign statement, got " + peek());
    }

    expect("=");

    shared_ptr<ExprNode> exprNode = parseExprNode();

    //! To check for syntax error
    expect(";");
    shared_ptr<AssignNode> assignNode = make_shared<AssignNode>(variableNode, exprNode);
    return assignNode;
}

shared_ptr<PrintNode> Parser::parsePrintNode() {
    int savedIdx = currIdx;
    if (!match("print")) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<VariableNode> varNode = parseVariableNode();
    if (!varNode) {
        currIdx = savedIdx;
        return nullptr;
    }
    //! To check for syntax error
    expect(";");

    shared_ptr<PrintNode> printNode = make_shared<PrintNode>(varNode);
    return printNode;
}

shared_ptr<WhileNode> Parser::parseWhileNode() {
    int savedIdx = currIdx;
    if (!match("while")) {
        currIdx = savedIdx;
        return nullptr;
    }

    //! while can be a variable name
    if (!match("(")) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<CondExprNode> condExprNode = parseCondExprNode();

    expect(")");
    expect("{");
    vector<shared_ptr<StmtNode>> statementList = parseStatementList();
    expect("}");
    shared_ptr<WhileNode> whileNode = make_shared<WhileNode>(condExprNode, statementList);
    return whileNode;
}

shared_ptr<CallNode> Parser::parseCallNode() {
    int savedIdx = currIdx;
    if (!match("call")) {
        currIdx = savedIdx;
        return nullptr;
    }

    string programName = pop();
    if (!Utils::isValidName(programName)) {
        currIdx = savedIdx;
        return nullptr;
    }
    //! To check for syntax error
    expect(";");

    shared_ptr<CallNode> callNode = make_shared<CallNode>(programName);
    return callNode;
}

shared_ptr<VariableNode> Parser::parseVariableNode() {
    int savedIdx = currIdx;
    string varName = pop();
    if (!Utils::isValidName(varName)) {
        currIdx = savedIdx;
        return nullptr;
    }
    shared_ptr<VariableNode> variableNode = make_shared<VariableNode>(varName);
    return variableNode;
}

shared_ptr<ExprNode> Parser::parseExprNode() {
    //TODO: store the string for now, will convert to a ASTNode in the future

    string expr = pop();
    while (peek().compare(";") != 0) {
        expr += pop();
    }
    return make_shared<ExprNode>(expr);
}

shared_ptr<CondExprNode> Parser::parseCondExprNode() {
    //TODO: need to check for correct condition, e.g. unexpected token is not allowed
    string condExpr = pop();
    while (peek().compare(")") != 0) {
        if (Utils::isValidName(peek()) ||
                Utils::isValidNumber(peek()) ||
            allowedTokenForCondExpr.count(peek())) {
            condExpr += pop();
        } else {
            throw SPParseException("Invalid unexpected token in conditional expression: " + peek());
        }
    }
    shared_ptr<CondExprNode> condExprNode = make_shared<CondExprNode>(condExpr);
    return condExprNode;
}

shared_ptr<ASTNode> Parser::parse() {
    vector<shared_ptr<ProcedureNode>> procedureList;
    while (currIdx < tokens.size()) {
        //! process each procedure recursively and add to the list
        shared_ptr<ProcedureNode> procedure = parseProcedureNode();
        procedureList.push_back(procedure);
    }
    if (procedureList.size() == 0) {
        throw SPParseException("No procedure found.");
    }
    shared_ptr<ASTNode> root = make_shared<ProgramNode>(procedureList);
    return root;
}
