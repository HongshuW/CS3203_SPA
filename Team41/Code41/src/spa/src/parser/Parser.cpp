//
// Created by Tan Wei En on 2/9/22.
//

#include "Parser.h"
#include "SPExceptions.h"

using namespace SourceParser;
using namespace std;

// consider creating Token class instead of string for tokens.

Parser::Parser(vector<string> tokens)
    : currIdx(0), tokens(tokens) {};

string Parser::peek() { return tokens[currIdx]; }

string Parser::peekNext() { return tokens[currIdx + 1]; }

string Parser::previous() { return tokens[currIdx - 1]; }

string Parser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool Parser::equals(string s1, string s2) {
    return s1 == s2;
}

bool Parser::match(string s) {
    if (peek() == s) {
        currIdx++;
        return true;
    }
    return false;
}

bool Parser::expect(string s) {
    if (match(s)) {
        return true;
    }
    string errorMessage = ErrorMessageFormatter::formatErrorMessage(s, peek());
    throw SPParseException(errorMessage);
}

shared_ptr<ProcedureNode> Parser::parseProcedureNode() {
    expect(ParserConstants::PROCEDURE);
    //! need to check for valid procedureName
    string procedureName = pop();
    if (!Utils::isValidName(procedureName)) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_PROC_NAME + procedureName);
    }
    expect(ParserConstants::CURLY_LEFT_BRACKET);
    vector<shared_ptr<StmtNode>> statementList = parseStatementList();
    expect(ParserConstants::CURLY_RIGHT_BRACKET);

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
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_EMPTY_STMT_LIST);
    }
    return statementList;
}

shared_ptr<StmtNode> Parser::parseStatementNode() {
    //! if, read, assign, print, while, call
    if (peek() == ParserConstants::CURLY_RIGHT_BRACKET) {
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
    if (!match(ParserConstants::IF)) {
        currIdx = savedIdx;
        return nullptr;
    }

    if (!match(ParserConstants::LEFT_BRACKET)) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<CondExprNode> condExprNode = parseCondExprNode();

    if (!condExprNode) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION, peek());
        throw SPParseException(errorMessage);
    }

    expect(ParserConstants::THEN);
    expect(ParserConstants::CURLY_LEFT_BRACKET);
    vector<shared_ptr<StmtNode>> ifStatementList = parseStatementList();
    expect(ParserConstants::CURLY_RIGHT_BRACKET);
    expect(ParserConstants::ELSE);
    expect(ParserConstants::CURLY_LEFT_BRACKET);
    vector<shared_ptr<StmtNode>> elseStatementList = parseStatementList();
    expect(ParserConstants::CURLY_RIGHT_BRACKET);
    shared_ptr<IfNode> ifNode = make_shared<IfNode>(condExprNode,
                                                    ifStatementList, elseStatementList);
    return ifNode;
}

shared_ptr<ReadNode> Parser::parseReadNode() {
    int savedIdx = currIdx;
    if (!match(ParserConstants::READ)) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<VariableNode> varNode = parseVariableNode();

    if (!varNode) {
        currIdx = savedIdx;
        return nullptr;
    }
    //! To check for syntax error
    expect(ParserConstants::SEMICOLON);
    shared_ptr<ReadNode> readNode = make_shared<ReadNode>(varNode);
    return readNode;
}

shared_ptr<AssignNode> Parser::parseAssignNode() {
    shared_ptr<VariableNode> variableNode = parseVariableNode();

    if (!variableNode) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_PARSE_EXCEPTION_EXPECT_VARIABLE_NAME, peek());
        throw SPParseException(errorMessage);
    }

    expect(ParserConstants::EQUAL);

    shared_ptr<ExprNode> exprNode = parseExprNode();

    //! To check for syntax error
    expect(ParserConstants::SEMICOLON);
    shared_ptr<AssignNode> assignNode = make_shared<AssignNode>(variableNode, exprNode);
    return assignNode;
}

shared_ptr<PrintNode> Parser::parsePrintNode() {
    int savedIdx = currIdx;
    if (!match(ParserConstants::PRINT)) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<VariableNode> varNode = parseVariableNode();
    if (!varNode) {
        currIdx = savedIdx;
        return nullptr;
    }
    //! To check for syntax error
    expect(ParserConstants::SEMICOLON);

    shared_ptr<PrintNode> printNode = make_shared<PrintNode>(varNode);
    return printNode;
}

shared_ptr<WhileNode> Parser::parseWhileNode() {
    int savedIdx = currIdx;
    if (!match(ParserConstants::WHILE)) {
        currIdx = savedIdx;
        return nullptr;
    }

    //! while can be a variable name
    if (!match(ParserConstants::LEFT_BRACKET)) {
        currIdx = savedIdx;
        return nullptr;
    }

    shared_ptr<CondExprNode> condExprNode = parseCondExprNode();

    expect(ParserConstants::CURLY_LEFT_BRACKET);
    vector<shared_ptr<StmtNode>> statementList = parseStatementList();
    expect(ParserConstants::CURLY_RIGHT_BRACKET);
    shared_ptr<WhileNode> whileNode = make_shared<WhileNode>(condExprNode, statementList);
    return whileNode;
}

shared_ptr<CallNode> Parser::parseCallNode() {
    int savedIdx = currIdx;
    if (!match(ParserConstants::CALL)) {
        currIdx = savedIdx;
        return nullptr;
    }

    string programName = pop();
    if (!Utils::isValidName(programName)) {
        currIdx = savedIdx;
        return nullptr;
    }
    //! To check for syntax error
    expect(ParserConstants::SEMICOLON);

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
    vector<string> expr;
    expr.push_back(pop());
    while (peek() != ParserConstants::SEMICOLON) {
        expr.push_back(pop());
    }
    if (expr.empty()) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY);
    }
    ExprNodeParser exprNodeParser = ExprNodeParser(expr);
    shared_ptr<ExprNode> exprNode = exprNodeParser.parse();
    if (!exprNode) {
        // throw error
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY);
    }
    return exprNode;
}

shared_ptr<CondExprNode> Parser::parseCondExprNode() {
    vector<string> condExpr;
    bool isConnected = false;
    while (!(equals(peek(), ParserConstants::RIGHT_BRACKET)
            && (equals(peekNext(), ParserConstants::THEN)
                || equals(peekNext(), ParserConstants::CURLY_LEFT_BRACKET)))) {
        string curr = pop();
        if (!Utils::isValidName(curr) && !Utils::isValidNumber(curr)) {
            //! is a comparator operator, need to check for valid operator
            if (!Utils::VALID_TOKENS_COND_EXPR.count(curr)) {
                string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                        ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP, curr);
                throw SPParseException(errorMessage);
            }
        }
        //! Check for the presence of && and ||
        if (Utils::isConnector(curr)) {
            isConnected = true;
        }
        condExpr.push_back(curr);
    }

    //! remove the ending )
//    if (previous().compare(")") == 0) {
//        condExpr.pop_back();
//    }
    pop();

    //! Check for valid parentheses
    if (!Utils::isValidParentheses(condExpr)) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_PARENTHESES);
    }

    CondExprParser condExprParser = CondExprParser(condExpr, isConnected);
    //! If the tree can be built without any errors,
    //! there is not syntax error and we can safely
    //! using the string representation of the conditional expression.
    condExprParser.parse();
    string condExprStr = ParserConstants::EMPTY_STR;
    for (auto str : condExpr) {
        condExprStr += str;
    }
    return make_shared<CondExprNode>(condExprStr);
}

shared_ptr<ProgramNode> Parser::parse() {
    vector<shared_ptr<ProcedureNode>> procedureList;
    while (currIdx < tokens.size()) {
        //! process each procedure recursively and add to the list
        shared_ptr<ProcedureNode> procedure = parseProcedureNode();
        procedureList.push_back(procedure);
    }
    if (procedureList.empty()) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_EMPTY_PROCEDURE);
    }
    shared_ptr<ProgramNode> root = make_shared<ProgramNode>(procedureList);
    return root;
}
