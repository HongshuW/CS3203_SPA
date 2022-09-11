//
// Created by Xingchen Lin on 11/9/22.
//

#include "CondExprParser.h"

using namespace SourceParser;

CondExprParser::CondExprParser(vector<string>& tokens) : tokens(tokens), currIdx(0) {};

string CondExprParser::peek() { return tokens[currIdx]; }

string CondExprParser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool CondExprParser::match(string s) {
    if (peek().compare(s) == 0) {
        currIdx++;
        return true;
    }
    return false;
}

bool CondExprParser::equals(string s1, string s2) {
    if (s1.compare(s2) == 0) {
        return true;
    }
    return false;
}

bool CondExprParser::expect(string s) {
    if (match(s)) {
        return true;
    }
    throw SPParseException("Expect '" + s + "', got '" + peek() + "'.");
}

shared_ptr<ExprNode> CondExprParser::parseExprNodeForRelExpr(string direction) {
    vector<string> expr;
    expr.push_back(pop());
    if (direction == "left") {
        while (currIdx < tokens.size() && Utils::allowedTokenForRelExprNode.count(peek()) == 0) {
            if (!Utils::isValidNumber(peek())
                    && !Utils::isValidName(peek())
                    && !Utils::isMathOperators(peek())) {
                throw SPParseException("Expect a comparator operator, got: " + peek());
            }
            expr.push_back(pop());
        }
    } else {
        while (currIdx < tokens.size() && peek().compare(")") != 0) {
            expr.push_back(pop());
        }
    }

    if (expr.size() <= 0) {
        throw SPParseException("The relational expression cannot be empty");
    }
    ExprNodeParser exprNodeParser = ExprNodeParser(expr);
    shared_ptr<ExprNode> exprNode = exprNodeParser.parse();
    return exprNode;
}

shared_ptr<RelExprNode> CondExprParser::parseRelExprNode() {
    shared_ptr<ExprNode> exprNodeLHS = parseExprNodeForRelExpr("left");
    string op = peek();
    if (Utils::allowedTokenForRelExprNode.count(peek()) == 0) {
        throw SPParseException("Expect a comparator operator, got: " + op);
    }
    currIdx++;
    shared_ptr<ExprNode> exprNodeRHS = parseExprNodeForRelExpr("right");
    shared_ptr<RelExprNode> relExprNode = make_shared<RelExprNode>(exprNodeLHS, op, exprNodeRHS);
    return relExprNode;
}

shared_ptr<CondExprNode> CondExprParser::parse() {
    if (peek().compare("!") == 0) {
        //! !(condExpr)
        currIdx++;
        expect("(");
        shared_ptr<CondExprNode> condExprNode = parse();
        expect(")");
        return make_shared<CondExprNode>(condExprNode);
    } else if (peek().compare("(") == 0) {
        //! (condExpr) && (condExpr)
        //! (condExpr) || (condExpr)
        expect("(");
        shared_ptr<CondExprNode> condExprLHS = parse();
        expect(")");

        string op;

        if (match("&&")) {
            op = "&&";
        } else if (match("||")) {
            op = "||";
        } else {
            throw SPParseException("Expected '||' or '&&', got: " + peek());
        }

        expect("(");
        shared_ptr<CondExprNode> condExprRHS = parse();
        expect(")");
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

    throw SPParseException("Syntax errors detected in conditional expression");
}