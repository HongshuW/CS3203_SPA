//
// Created by Xingchen Lin on 10/9/22.
//

#include "ExprNodeParser.h"

ExprNodeParser::ExprNodeParser(vector<string>& tokens) : tokens(tokens), currIdx(ParserConstants::ZERO) {}

string ExprNodeParser::peek() { return tokens[currIdx]; }

string ExprNodeParser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool ExprNodeParser::match(const string& s) {
    return peek() == s;
}

int ExprNodeParser::getPriority(const string& s) {
    if (s == ParserConstants::TIMES || s == ParserConstants::DIVIDE || s == ParserConstants::MODULO) {
        return ParserConstants::TWO;
    } else if (s == ParserConstants::PLUS || s == ParserConstants::MINUS) {
        return ParserConstants::ONE;
    } else {
        // "(" or digit or synonym
        return ParserConstants::ZERO;
    }
}

bool ExprNodeParser::buildTree(const string& operatorStack, const shared_ptr<stack<shared_ptr<ExprNode>>>& nodeStack) {
    shared_ptr<ExprNode> rootNode = make_shared<ExprNode>(operatorStack);
    if (nodeStack->empty()) return false;
    rootNode->right = nodeStack->top();
    nodeStack->pop();
    if (nodeStack->empty()) return false;
    rootNode->left = nodeStack->top();
    nodeStack->pop();
    nodeStack->push(rootNode);
    return true;
}

shared_ptr<ExprNode> ExprNodeParser::parse() {
    shared_ptr<stack<string>> operatorStack = make_shared<stack<string>>();
    shared_ptr<stack<shared_ptr<ExprNode>>> nodeStack = make_shared<stack<shared_ptr<ExprNode>>>();

    while (currIdx < tokens.size()) {
        if (match(ParserConstants::LEFT_BRACKET)) {
            operatorStack->push(pop());
        } else if (match(ParserConstants::RIGHT_BRACKET)) {
            // pop until "("
            while (!operatorStack->empty() && operatorStack->top() != ParserConstants::LEFT_BRACKET) {
                buildTree(operatorStack->top(), nodeStack);
                operatorStack->pop();
            }
            // pop the "(" symbol
            operatorStack->pop();
            pop();
        } else if (getPriority(peek()) > ParserConstants::ZERO) {
            // is operator
            while (!operatorStack->empty() && getPriority(operatorStack->top()) >= getPriority(peek())) {
                buildTree(operatorStack->top(), nodeStack);
                operatorStack->pop();
            }
            operatorStack->push(pop());
        } else {
            // is digit or synonym
            nodeStack->push(make_shared<ExprNode>(pop()));
        }
    }

    while (!operatorStack->empty()) {
        if (!buildTree(operatorStack->top(), nodeStack)) {
            return nullptr;
        }
        operatorStack->pop();
    }

    if (nodeStack->empty()) {
        return nullptr;
    }

    return nodeStack->top();
}