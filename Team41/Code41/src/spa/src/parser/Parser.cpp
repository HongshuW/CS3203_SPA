//
// Created by Tan Wei En on 2/9/22.
//

#include "Parser.h"
#include "Exceptions.h"
#include "iostream"
#include "LexicalAnalyser.h"

using namespace SourceParser;
using namespace std;

// consider creating Token class instead of string for tokens.

Parser::Parser(vector<string> tokens)
    : currIdx(0), tokens(tokens) {};

string Parser::peek() {
    return tokens[currIdx];
}

string Parser::pop() {
    string currToken = tokens[currIdx];
    currIdx++;
    return currToken;
}

string Parser::previous() {
    if (currIdx == 0) {
        return "No previous token";
    }

    return tokens[currIdx - 1];
}

bool Parser::match(string s) {
    if (peek().compare(s) == 0) {
        return true;
    }
    return false;
}

bool Parser::expect(string s) {
    if (match(s)) {
        currIdx++;
        return true;
    }
    throw SPParseException("Expect '" + s + "', got '" + peek() + "'.");
}

shared_ptr<ProcedureNode> Parser::parseProcedure() {
    expect("procedure");
    string procedureName = pop();
    expect("{");
    auto statementList = parseStatementList();
    expect("}");

    // include check to check if statementList is empty
    return make_shared<ProcedureNode>(procedureName, statementList);
}

shared_ptr<AssignNode> Parser::parseAssign() {
    auto variable = parseVariable();
    expect("=");
    auto expression = parseExpression();
    expect(";");
    return make_shared<AssignNode>(move(variable), move(expression));
}

shared_ptr<VariableNode> Parser::parseVariable() {
    // can include expect(variable) maybe
    string variableName = pop();
    auto result = make_shared<VariableNode>(variableName);
    return result;
}

shared_ptr<NumberNode> Parser::parseNumber() {
    string number = pop();
    auto result = make_shared<NumberNode>(number);
    return result;
}

shared_ptr<ExpressionNode> Parser::parseExpression() {
    // expr: expr + term | expr - term | term
}

shared_ptr<TermNode> Parser::parseTerm() {
    // term: term ‘*’ factor | term ‘/’ factor | term ‘%’ factor | factor
    auto factor = parseFactor();
    while (currIdx < tokens.size()) {
        if (match("*") || match("/") || match("%")) {
            string op = pop();
            // what to do here?
            auto term = parseTerm();


        } else {
            break;
        }
    }
}


// todo set the pointers of factorNode when returning
shared_ptr<FactorNode> Parser::parseFactor() {
    // factor: var_name | const_value | ‘(’ expr ‘)’
    auto factorNode = make_shared<FactorNode>();
    string next = peek();
    if (match("(")) {
        auto expression = parseExpression();
        expect(")");
        // set factorNode -> expression
    } else if (LexicalAnalyser::isValidName(next)) {
        auto variable = parseVariable();
    } else if (LexicalAnalyser::isValidInteger(next)) {
        auto number = parseNumber();
    }

    return factorNode;

}

shared_ptr<ProgramNode> Parser::parseProgram() {
    vector<shared_ptr<ProcedureNode>> procedureList;
    while (currIdx < tokens.size()) {
        auto procedure = parseProcedure();
        procedureList.push_back(procedure);
    }
    auto programNode = make_shared<ProgramNode>(procedureList);
    return programNode;
}
