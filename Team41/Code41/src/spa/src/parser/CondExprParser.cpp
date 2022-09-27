//
// Created by Xingchen Lin on 11/9/22.
//

#include "CondExprParser.h"
#include "parser/constants/ParserConstants.h"
#include "utils/ErrorMessageFormatter.h"
using namespace SourceParser;

CondExprParser::CondExprParser(vector<string>& tokens, bool isConnected) : isConnected(isConnected),
tokens(tokens) {};

string CondExprParser::peek() { return tokens[currIdx]; }

string CondExprParser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool CondExprParser::match(string s) {
    if (peek() == s) {
        currIdx++;
        return true;
    }
    return false;
}

bool CondExprParser::equals(string s1, string s2) {
    if (s1 == s2) {
        return true;
    }
    return false;
}

bool CondExprParser::expect(string s) {
    if (match(s)) {
        return true;
    }
    string errorMessage = ErrorMessageFormatter::formatErrorMessage(s, peek());
    throw SPParseException(errorMessage);
}

shared_ptr<ExprNode> CondExprParser::parseExprNodeForRelExpr(string direction) {
    vector<string> expr;
    //! We need to insert the correct number of right brackets at the end
    unsigned int leftBracketCount = 0;
    if (peek() == ParserConstants::LEFT_BRACKET) leftBracketCount++;

    expr.push_back(pop());
    if (direction == ParserConstants::LEFT_DIRECTION) {
        while (currIdx < tokens.size() && !Utils::VALID_TOKENS_REL_EXPR.count(peek())) {
            if (!Utils::isValidNumber(peek())
                    && !Utils::isValidName(peek())
                    && !Utils::isMathOperators(peek())
                    && !Utils::isBracket(peek())) {
                string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                        ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP, peek());
                throw SPParseException(errorMessage);
            }
            expr.push_back(pop());
        }
    } else {
        while (currIdx < tokens.size() && peek() != ParserConstants::RIGHT_BRACKET) {
            if (!Utils::isValidNumber(peek())
                && !Utils::isValidName(peek())
                && !Utils::isMathOperators(peek())
                && !Utils::isBracket(peek())) {
                string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                        ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP, peek());
                throw SPParseException(errorMessage);
            }
            if (peek() == ParserConstants::LEFT_BRACKET) leftBracketCount++;
            expr.push_back(pop());
        }

        while (leftBracketCount) {
            expr.push_back(ParserConstants::RIGHT_BRACKET);
            leftBracketCount--;
        }
    }

    if (expr.empty()) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_EMPTY_RELATIONAL_EXPRESSION);
    }
    ExprNodeParser exprNodeParser = ExprNodeParser(expr);
    shared_ptr<ExprNode> exprNode = exprNodeParser.parse();
    //! if exprNode == nullptr, we are able to build the exprTree
    if (!exprNode) {
        throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_INVALID_CONDITIONAL_EXPRESSION);
    }
    return exprNode;
}

shared_ptr<RelExprNode> CondExprParser::parseRelExprNode() {
    shared_ptr<ExprNode> exprNodeLHS = parseExprNodeForRelExpr(ParserConstants::LEFT_DIRECTION);
    string op = peek();
    if (!Utils::VALID_TOKENS_REL_EXPR.count(peek())) {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                ParserConstants::SP_PARSE_EXCEPTION_EXPECT_COMPARATOR_OP, op);
        throw SPParseException(errorMessage);
    }
    currIdx++;
    shared_ptr<ExprNode> exprNodeRHS = parseExprNodeForRelExpr(ParserConstants::RIGHT_DIRECTION);
    shared_ptr<RelExprNode> relExprNode = make_shared<RelExprNode>(exprNodeLHS, op, exprNodeRHS);
    return relExprNode;
}

shared_ptr<CondExprNode> CondExprParser::parse() {
    if (match(ParserConstants::EXCLAMATION_MARK)) {
        //! !(condExpr)
        expect(ParserConstants::LEFT_BRACKET);
        shared_ptr<CondExprNode> condExprNode = parse();
        expect(ParserConstants::RIGHT_BRACKET);
        return make_shared<CondExprNode>(condExprNode);
    } else if (peek() == ParserConstants::LEFT_BRACKET && isConnected) {
        //! (condExpr) && (condExpr)
        //! (condExpr) || (condExpr)
        expect(ParserConstants::LEFT_BRACKET);
        shared_ptr<CondExprNode> condExprLHS = parse();
        expect(ParserConstants::RIGHT_BRACKET);

        string op;

        if (match(ParserConstants::AND_OP)) {
            op = ParserConstants::AND_OP;
        } else if (match(ParserConstants::OR_OP)) {
            op = ParserConstants::OR_OP;
        } else {
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                    ParserConstants::AND_OP_OR_OR_OP, peek());
            throw SPParseException(errorMessage);
        }

        expect(ParserConstants::LEFT_BRACKET);
        shared_ptr<CondExprNode> condExprRHS = parse();
        expect(ParserConstants::RIGHT_BRACKET);
        shared_ptr<CondExprNode> condExprNode = make_shared<CondExprNode>(condExprLHS, op, condExprRHS);
        return condExprNode;
    } else {
        //! relExpr
        shared_ptr<RelExprNode> relExprNode = parseRelExprNode();
        if (relExprNode) {
            shared_ptr<CondExprNode> condExprNode = make_shared<CondExprNode>(relExprNode);
            return condExprNode;
        }
    }

    throw SPParseException(ParserConstants::SP_PARSE_EXCEPTION_SYNTAX_ERROR);
}