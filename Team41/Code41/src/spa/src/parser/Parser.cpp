//
// Created by Tan Wei En on 2/9/22.
//

#include "Parser.h"
#include <utility>
#include "SPExceptions.h"

using namespace SourceParser;
using namespace std;

Parser::Parser(vector<string> tokens)
    : currIdx(TokenizerConstants::ZERO), tokens(std::move(tokens)) {}

string Parser::peek() { return tokens[currIdx]; }


string Parser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool Parser::match(const string& s) {
    if (peek() == s) {
        currIdx++;
        return true;
    }
    return false;
}

bool Parser::expect(const string& s) {
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
        if (!statementNode) break;
        statementList.push_back(statementNode);
    }

    if (statementList.empty()) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_EMPTY_STMT_LIST);
    }
    return statementList;
}

shared_ptr<StmtNode> Parser::parseStatementNode() {
    //! if, read, assign, print, while, call
    if (peek() == ParserConstants::CURLY_RIGHT_BRACKET) return nullptr;

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

    shared_ptr<CondExprNode> condExprNode = parseCondExprNode(currIdx);

    if (!condExprNode) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION, peek());
        throw SPParseException(errorMessage);
    }

    expect(ParserConstants::RIGHT_BRACKET);
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

    shared_ptr<CondExprNode> condExprNode = parseCondExprNode(currIdx);

    if (!condExprNode) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COND_EXPRESSION, peek());
        throw SPParseException(errorMessage);
    }

    expect(ParserConstants::RIGHT_BRACKET);
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
    while (peek() != ParserConstants::SEMICOLON) {
        string curr = pop();
        if (Utils::isBracket(curr) || Utils::isMathOperators(curr) ||
            Utils::isValidName(curr) || Utils::isValidNumber(curr)) {
            //! We need to ensure all tokens are valid before passing to the ExprNodeParser
            expr.push_back(curr);
        } else {
            throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR);
        }
    }
    if (expr.empty()) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY);
    }
    ExprNodeParser exprNodeParser = ExprNodeParser(expr);
    shared_ptr<ExprNode> exprNode = exprNodeParser.parse();
    if (!exprNode) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_ASSIGN_STMT_RHS_EMPTY);
    }
    return exprNode;
}

shared_ptr<ExprNode> Parser::parseToken(const string& curr) {
    if (Utils::isValidNumber(curr) || Utils::isValidName(curr)) {
        return std::make_shared<ExprNode>(curr);
    } else if (curr == ParserConstants::LEFT_BRACKET) {
        shared_ptr<ExprNode> expr = parseRelFactor(ParserConstants::ZERO);
        expect(ParserConstants::RIGHT_BRACKET);
        return expr;
    } else {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR);
    }
}

int Parser::parseSymbol(const string& curr) {
    const unordered_set<string> ALLOWED_TOKENS = unordered_set<string>({
        ParserConstants::LESS_THAN,
        ParserConstants::LESS_THAN_AND_EQUAL_TO,
        ParserConstants::GREATER_THAN,
        ParserConstants::GREATER_THAN_AND_EQUAL_TO,
        ParserConstants::EQUAL_TO,
        ParserConstants::NOT_EQUAL_TO,
        ParserConstants::SEMICOLON,
        ParserConstants::RIGHT_BRACKET
    });

    if (ALLOWED_TOKENS.count(curr) ||
        currIdx >= tokens.size() || curr == ParserConstants::RIGHT_BRACKET) {
        return ParserConstants::ZERO;
    } else if (curr == ParserConstants::PLUS || curr == ParserConstants::MINUS) {
        return ParserConstants::ONE;
    } else if (curr == ParserConstants::TIMES || curr == ParserConstants::DIVIDE || curr == ParserConstants::MODULO) {
        return ParserConstants::TWO;
    } else {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR);
    }
}

shared_ptr<ExprNode> Parser::makeExprNode(const string& curr, const shared_ptr<ExprNode>& lhs) {
    if (curr == ParserConstants::PLUS) {
        shared_ptr<ExprNode> rhs = parseRelFactor(ParserConstants::ONE);
        shared_ptr<ExprNode> node = make_shared<ExprNode>(ParserConstants::PLUS);
        node->left = lhs;
        node ->right = rhs;
        return node;
    } else if (curr == ParserConstants::MINUS) {
        shared_ptr<ExprNode> rhs = parseRelFactor(ParserConstants::ONE);
        shared_ptr<ExprNode> node = make_shared<ExprNode>(ParserConstants::MINUS);
        node->left = lhs;
        node ->right = rhs;
        return node;
    } else if (curr == ParserConstants::TIMES) {
        shared_ptr<ExprNode> rhs = parseRelFactor(ParserConstants::TWO);
        shared_ptr<ExprNode> node = make_shared<ExprNode>(ParserConstants::TIMES);
        node->left = lhs;
        node ->right = rhs;
        return node;
    } else if (curr == ParserConstants::DIVIDE) {
        shared_ptr<ExprNode> rhs = parseRelFactor(ParserConstants::TWO);
        shared_ptr<ExprNode> node = make_shared<ExprNode>(ParserConstants::DIVIDE);
        node->left = lhs;
        node ->right = rhs;
        return node;
    } else if (curr == ParserConstants::MODULO) {
        shared_ptr<ExprNode> rhs = parseRelFactor(ParserConstants::TWO);
        shared_ptr<ExprNode> node = make_shared<ExprNode>(ParserConstants::MODULO);
        node->left = lhs;
        node ->right = rhs;
        return node;
    } else {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_TOKEN_EXPR);
    }
}

shared_ptr<ExprNode> Parser::parseRelFactor(int priority) {
    string curr = pop();
    shared_ptr<ExprNode> exprNode = parseToken(curr);
    while (parseSymbol(peek()) > priority) {
        curr = pop();
        exprNode = makeExprNode(curr, exprNode);
    }
    return exprNode;
}

shared_ptr<RelExprNode> Parser::parseRelExprNode() {
    auto lhs = parseRelFactor(ParserConstants::ZERO);

    unordered_set<string> valid_ops({
        ParserConstants::LESS_THAN,
        ParserConstants::LESS_THAN_AND_EQUAL_TO,
        ParserConstants::GREATER_THAN,
        ParserConstants::GREATER_THAN_AND_EQUAL_TO,
        ParserConstants::EQUAL_TO,
        ParserConstants::NOT_EQUAL_TO
    });

    auto op = peek();
    if (!valid_ops.count(op)) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP);
    }

    currIdx++;

    auto rhs = parseRelFactor(ParserConstants::ZERO);
    return std::make_shared<RelExprNode>(lhs, op, rhs);
}

shared_ptr<CondExprNode> Parser::parseCondExprNode(int startIdx) {
    if (match(ParserConstants::EXCLAMATION_MARK)) {
        expect(ParserConstants::LEFT_BRACKET);
        auto condExprNode = parseCondExprNode(startIdx - ParserConstants::ONE);
        expect(ParserConstants::RIGHT_BRACKET);
        return make_shared<CondExprNode>(condExprNode);
    } else if (peek() == ParserConstants::LEFT_BRACKET) {  // () op ()
        int tempStart = currIdx;
        expect(ParserConstants::LEFT_BRACKET);
        shared_ptr<CondExprNode> condLHS = nullptr;
        try {
            condLHS = parseCondExprNode(startIdx - ParserConstants::ONE);
        } catch (const SPParseException& e) {
            currIdx = tempStart;
            auto relExpr = parseRelExprNode();
            if (relExpr) {
                return std::make_shared<CondExprNode>(std::move(relExpr));
            }
        }
        expect(ParserConstants::RIGHT_BRACKET);

        string op;

        if (match(ParserConstants::AND_OP)) {
            op = ParserConstants::AND_OP;
        } else if (match(ParserConstants::OR_OP)) {
            op = ParserConstants::OR_OP;
        } else {
            return nullptr;
        }

        expect(ParserConstants::LEFT_BRACKET);
        auto condRHS = parseCondExprNode(startIdx - ParserConstants::ONE);
        expect(ParserConstants::RIGHT_BRACKET);

        return std::make_shared<CondExprNode>(std::move(condLHS), op,
                                              std::move(condRHS));
    } else {  // relExpr
        auto relExpr = parseRelExprNode();
        if (relExpr) {
            return std::make_shared<CondExprNode>(std::move(relExpr));
        }
    }
    // Shouldn't reach here
    return nullptr;
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

