//
// Created by Tan Wei En on 2/9/22.
//

#include "Parser.h"
#include "iostream"

using namespace SourceParser;
using namespace std;

Parser::Parser(std::vector<std::string> tokens)
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
        currIdx++;
        return true;
    }
    return false;
}

bool Parser::expect(string s) {
    if (match(s)) {
        return true;
    }
    // throw error here instead of returning false
    return false;
}

shared_ptr<ProcedureNode> Parser::parseProcedure() {
    expect("procedure");
    std::string procedureName = pop();
    expect("{");
    // todo change this to parseStatementList
    auto Assign = parseAssign();
    expect("}");

    return make_shared<ProcedureNode>(procedureName, Assign);
}

shared_ptr<AssignNode> Parser::parseAssign() {
    auto variable = parseVariable();
    expect("=");
    // will change to expression in future
    auto number = parseNumber();
    expect(";");
    return make_shared<AssignNode>(move(variable), move(number));
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

shared_ptr<ProgramNode> Parser::parse() {
    std::vector<std::shared_ptr<ProcedureNode>> procedureList;
    while (currIdx < tokens.size()) {
        auto procedure = parseProcedure();
        procedureList.push_back(procedure);
    }
    auto programNode = make_shared<ProgramNode>(procedureList);
    return programNode;
}
