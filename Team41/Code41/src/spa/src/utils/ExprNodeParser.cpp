//
// Created by Xingchen Lin on 10/9/22.
//

#include "ExprNodeParser.h"

ExprNodeParser::ExprNodeParser(vector<string>& tokens) : tokens(tokens), currIdx(0){};

string ExprNodeParser::peek() { return tokens[currIdx]; }


string ExprNodeParser::pop() {
    string currToken = peek();
    currIdx++;
    return currToken;
}

bool ExprNodeParser::match(string s) {
    if (peek().compare(s) == 0) {
        return true;
    }
    return false;
}

bool ExprNodeParser::equals(string s1, string s2) {
    if (s1.compare(s2) == 0) {
        return true;
    }
    return false;
}

int ExprNodeParser::getPriority(string s) {
    if (equals("*", s) || equals("/", s)) {
        return 2;
    } else if (equals("-", s) || equals("+", s)) {
        return 1;
    } else {
        // "(" or digit or synonym
        return 0;
    }
}

void ExprNodeParser::buildTree(const string opt, shared_ptr<stack<shared_ptr<ExprNode>>> opd) {
    shared_ptr<ExprNode> root = make_shared<ExprNode>(opt);
    root->right = opd->top();
    opd->pop();
    root->left = opd->top();
    opd->pop();
    opd->push(root);
}

shared_ptr<ExprNode> ExprNodeParser::parse() {
    shared_ptr<stack<string>> operatorStack = make_shared<stack<string>>();
    shared_ptr<stack<shared_ptr<ExprNode>>> nodeStack = make_shared<stack<shared_ptr<ExprNode>>>();

    while (currIdx < tokens.size()) {
        if (match("(")) {
            operatorStack->push(pop());
        } else if (match(")")) {
            // pop ou till "("
            while (!operatorStack->empty() && !equals("(", operatorStack->top())) {
                buildTree(operatorStack->top(), nodeStack);
                operatorStack->pop();
            }
            // pop the "(" symbol
            operatorStack->pop();
            pop();
        } else if (getPriority(peek()) > 0) {
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
        buildTree(operatorStack->top(), nodeStack);
        operatorStack->pop();
    }

    if (nodeStack->empty()) {
        return nullptr;
    }

    return nodeStack->top();
}