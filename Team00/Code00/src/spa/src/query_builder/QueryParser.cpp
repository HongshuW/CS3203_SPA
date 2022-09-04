//
// Created by Xingchen Lin on 28/8/22.
//

#include "query_builder/QueryParser.h"
#include "query_builder/commons/Synonym.h"
#include "Exceptions.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/SuchThatClause.h"

using namespace QB;

QueryParser::QueryParser(std::vector<std::string> tokens)
        : query(new Query()), currIdx(0), tokens(tokens) {};

string QueryParser::peek() { return tokens[currIdx]; }

string QueryParser::previous() { return tokens[currIdx - 1]; }

string QueryParser::pop() {
    string currToken = tokens[currIdx];
    currIdx++;
    return currToken;
}

bool QueryParser::match(string s) {
    if (peek().compare(s) == 0) {
        currIdx++;
        return true;
    }
    return false;
}

bool QueryParser::expect(string s) {
    if (match(s)) {
        return true;
    }
    throw PQLParseException("Expect '" + s + "', got '" + peek() + "'.");
}

bool QueryParser::parseDeclarations() {
    unsigned int savedIdx = currIdx;
    std::vector<Synonym> synonymList;
    std::string designEntityStr = pop();
    DesignEntity designEntity;

    try {
        designEntity = getDesignEntity(designEntityStr);
    } catch (const PQLParseException& e) {
        currIdx = savedIdx;
        return false;
    }

    std::string synonymStr = pop();
    //! Throw syntax error if synonym is invalid
    Synonym synonym = Synonym(synonymStr);
    synonymList.push_back(synonym);
    while(!match(";")) {
        expect(",");
        synonymStr = pop();
        synonym = Synonym(synonymStr);
        synonymList.push_back(synonym);
    }

    for (const Synonym syn : synonymList) {
        Declaration declaration = Declaration(designEntity, syn);
        query->declarations->push_back(declaration);
    }

    return true;
}

void QueryParser::parseSelectClause() {
    std::string synonymStr = pop();
    //! Throw syntax error if synonym is invalid
    Synonym synonym = Synonym(synonymStr);
    SelectClause* selectClause = new SelectClause(synonym);
    query->selectClause = selectClause;
}

Ref QueryParser::parseRef() {
    //! Can be synonym, _, integer, or string
    if (match("_")) {
        return Underscore();
    } else if (match("\"")) {
        string identStr = pop();
        expect("\"");
        Ident ident = Ident(identStr);
        return ident;
    } else if (isDigit(peek())) {
        return std::stoi(pop());
    } else if (Synonym::isValidSynonym(peek())) {
        string synonymStr = pop();
        Synonym synonym = Synonym(synonymStr);
        return synonym;
    } else {
        throw PQLParseException("Expecting a Ref, got " + peek());
    }
}

bool QueryParser::isDigit(const string &str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

void QueryParser::parseSuchThatClause() {
    if (!match("such")) {
        throw PQLParseException("Unexpected token '" + peek() + "'");
    }

    expect("that");

    string relationTypeStr = pop();
    RelationType relationType = getRelationTypeFromStr(relationTypeStr);

    expect("(");
    auto arg1 = parseRef();
    expect(",");
    auto arg2 = parseRef();
    expect(")");

    SuchThatClause* suchThatClause = new SuchThatClause(relationType, arg1, arg2, query->declarations);
    query->suchThatClauses->push_back(suchThatClause);
}

Query QueryParser::parse() {
    while (currIdx < tokens.size()) {
        while (currIdx < tokens.size()) {
            if (!parseDeclarations()) break;
        }

        //! Throw syntax error if curr != "Select"
        expect("Select");

        parseSelectClause();

        while (currIdx < tokens.size()) {
            parseSuchThatClause();
            currIdx++;
            //! Throw syntax error accordingly
        }
    }
    return *query;
}
