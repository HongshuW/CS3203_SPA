//
// Created by Xingchen Lin on 28/8/22.
//

#include "query_builder/QueryParser.h"
#include "query_builder/commons/Synonym.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_builder/exceptions/Exceptions.h"

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
    shared_ptr<SelectClause> selectClause = make_shared<SelectClause>(synonym);
    query->selectClause = selectClause;
}

Ref QueryParser::parseRef() {
    //! Can be synonym, _, integer, or ident
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

bool QueryParser::parseSuchThatClause() {
    unsigned int savedIdx = currIdx;
    if (!match("such")) {
        currIdx = savedIdx;
        return false;
    }

    expect("that");

    string relationTypeStr = pop();
    RelationType relationType = getRelationTypeFromStr(relationTypeStr);

    expect("(");
    auto arg1 = parseRef();
    expect(",");
    auto arg2 = parseRef();
    expect(")");

    shared_ptr<SuchThatClause> suchThatClause =
        make_shared<SuchThatClause>(relationType, arg1, arg2, query->declarations);
    query->suchThatClauses->push_back(suchThatClause);
    return true;
}

ExpressionSpec QueryParser::parseExpressionSpec() {
    //TODO: need to convert "x+y+z" to vector of string in the future
    vector<string> expr;
    if (match("\"")) {
        //! Full match
        expr.push_back(pop());
        expect("\"");
        ExprNodeParser parser = ExprNodeParser(expr);
        return ExpressionSpec(ExpressionSpecType::FULL_MATCH, parser.parse());
    } else if (match("_")) {
        if (peek().compare(")") == 0) {
            //! Any match
            return ExpressionSpec(ExpressionSpecType::ANY_MATCH);
        } else {
            //! Partial match
            expect("\"");
            expr.push_back(pop());
            expect("\"");
            expect("_");
            ExprNodeParser parser = ExprNodeParser(expr);
            return ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, parser.parse());
        }
    } else {
        throw PQLParseException("Expect an expression, got " + peek());
    }
}

bool QueryParser::parsePatternClause() {
    unsigned int savedIdx = currIdx;
    if (!match("pattern")) {
        currIdx = savedIdx;
        return false;
    }

    Synonym arg1 = Synonym(pop());
    expect("(");
    // can be synonym, _, ident
    Ref arg2 = parseRef();
    expect(",");
    ExpressionSpec arg3 = parseExpressionSpec();
    expect(")");

    shared_ptr<PatternClause> patternClause = make_shared<PatternClause>(arg1, arg2, arg3);
    query->patternClause = patternClause;
    return true;
}

shared_ptr<Query> QueryParser::parse() {
    while (currIdx < tokens.size()) {
        while (currIdx < tokens.size()) {
            if (!parseDeclarations()) break;
        }

        //! Throw syntax error if curr != "Select"
        expect("Select");

        parseSelectClause();

        while (currIdx < tokens.size()) {
            if (parseSuchThatClause()) continue;
            if (parsePatternClause()) continue;
            //! Throw syntax error accordingly
            throw PQLParseException("Expect a such that or pattern clause, got " + peek());
        }
    }
    return query;
}
