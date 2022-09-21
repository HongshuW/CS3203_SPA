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

bool QueryParser::isWithinBound() {
    return currIdx < tokens.size();
}

bool QueryParser::match(string s) {
    if (isWithinBound() && peek().compare(s) == 0) {
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

void QueryParser::parseBooleanSelectClause() {
    //! If BOOLEAN is declared as a synonym in a PQL query, this declaration takes precedence
    shared_ptr<SelectClause> selectClause;
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    Synonym syn = Synonym("BOOLEAN");
    if (!Declaration::findDeclaration(syn, query->declarations)) {
        //! BOOLEAN is not declared as a synonym, this is a BOOLEAN type
        selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);
    } else {
        returnResults->push_back(syn);
        selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
    }
    query->selectClause = selectClause;
}

Elem QueryParser::parseTupleSelectClause() {
    string synonymStr = pop();
    Synonym synonym = Synonym(synonymStr);
    if (match(".")) {
        //! AttrRef
        string attrNameStr = pop();
        AttrName attrName = AttrRef::getAttrNameFromStr(attrNameStr);
        AttrRef attrRef = AttrRef(synonym, attrName);
        return attrRef;
    } else {
        //! Synonym
        return synonym;
    }
}

void QueryParser::parseSelectClause() {
    //! For BOOLEAN
    if (match("BOOLEAN")) {
        parseBooleanSelectClause();
        return;
    }

    shared_ptr<SelectClause> selectClause;
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    //! For Tuple
    if (match("<")) {
        while (!match(">")) {
            Elem elem = parseTupleSelectClause();
            returnResults->push_back(elem);
            selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
            query->selectClause = selectClause;
            if (match(">")) return;
            expect(",");
        }
    } else {
        //! For Single Synonym
        Elem elem = parseTupleSelectClause();
        returnResults->push_back(elem);
        selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
        query->selectClause = selectClause;
    }
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
    } else if (Utils::isValidNumber(peek())) {
        return std::stoi(pop());
    } else if (Utils::isValidName(peek())) {
        string synonymStr = pop();
        Synonym synonym = Synonym(synonymStr);
        return synonym;
    } else {
        throw PQLParseException("Expecting a Ref, got " + peek());
    }
}

void QueryParser::parseSuchThatClause() {
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
}

bool QueryParser::parseSuchThat() {
    if (!match("such")) return false;
    expect("that");
    parseSuchThatClause();
    while(match("and")) {
        parseSuchThatClause();
    }
    return true;
}

ExpressionSpec QueryParser::parseExpressionSpec() {
    vector<string> expr;
    if (match("\"")) {
        //! Full match
        ExprStringTokenizer tokenizer = ExprStringTokenizer(pop());
        expr = tokenizer.tokenize();
        if (expr.size() == 0) {
            throw PQLParseException("Expression for assign statement cannot be empty");
        }
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
            ExprStringTokenizer tokenizer = ExprStringTokenizer(pop());
            expr = tokenizer.tokenize();
            if (expr.size() == 0) {
                throw PQLParseException("Expression for assign statement cannot be empty");
            }
            expect("\"");
            expect("_");
            ExprNodeParser parser = ExprNodeParser(expr);
            return ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, parser.parse());
        }
    } else {
        throw PQLParseException("Expect an expression, got " + peek());
    }
}

void QueryParser::parsePatternClause() {
    Synonym arg1 = Synonym(pop());
    auto declaration = Declaration::findDeclaration(arg1, query->declarations);
    if (!declaration) {
        throw PQLValidationException("Synonym " + arg1.synonym + " is not declared for Pattern Clause");
    }
    expect("(");
    shared_ptr<PatternClause> patternClause;
    DesignEntity de = declaration->getDesignEntity();
    if (de == DesignEntity::ASSIGN) {
        Ref arg2 = parseRef();
        expect(",");
        ExpressionSpec arg3 = parseExpressionSpec();
        patternClause = make_shared<PatternClause>(DesignEntity::ASSIGN, arg1, arg2, arg3);
    } else if (de == DesignEntity::IF) {
        Ref arg2 = parseRef();
        expect(",");
        expect("_");
        expect(",");
        expect("_");
        patternClause = make_shared<PatternClause>(DesignEntity::IF, arg1, arg2);
    } else if (de == DesignEntity::WHILE) {
        Ref arg2 = parseRef();
        expect(",");
        expect("_");
        patternClause = make_shared<PatternClause>(DesignEntity::WHILE, arg1, arg2);
    } else {
        throw PQLParseException(getDesignEntityString(de) + " is not supported for Pattern Clause");
    }
    expect(")");
    query->patternClauses->push_back(patternClause);
}

bool QueryParser::parsePattern() {
    if (!match("pattern")) return false;
    parsePatternClause();
    while(match("and")) {
        parsePatternClause();
    }
    return true;
}

WithRef QueryParser::parseWithRef() {
    //! Can be ident, integer, or AttrRef
    if (Utils::isValidNumber(peek())) {
        return std::stoi(pop());
    } else if (match("\"")) {
        string identStr = pop();
        expect("\"");
        Ident ident = Ident(identStr);
        return ident;
    } else if (Utils::isValidName(peek())) {
        string synonymStr = pop();
        Synonym synonym = Synonym(synonymStr);
        expect(".");
        string attrNameStr = pop();
        AttrName attrName = AttrRef::getAttrNameFromStr(attrNameStr);
        AttrRef attrRef = AttrRef(synonym, attrName);
        return attrRef;
    } else {
        throw PQLParseException("Expecting a WithRef, got " + peek());
    }
}

void QueryParser::parseWithClause() {
    WithRef lhs = parseWithRef();
    expect("=");
    WithRef rhs = parseWithRef();
    shared_ptr<WithClause> withClause = make_shared<WithClause>(lhs, rhs);
    query->withClauses->push_back(withClause);
}

bool QueryParser::parseWith() {
    if (!match("with")) return false;
    parseWithClause();
    while(match("and")) {
        parseWithClause();
    }
    return true;
}

shared_ptr<Query> QueryParser::parse() {
    while (isWithinBound()) {
        while (isWithinBound()) {
            if (!parseDeclarations()) break;
        }

        //! Throw syntax error if curr != "Select"
        expect("Select");

        parseSelectClause();

        while (isWithinBound()) {
            if (parseSuchThat()) continue;
            if (parsePattern()) continue;
            if (parseWith()) continue;
            //! Throw syntax error accordingly
            throw PQLParseException("Expect a such that or pattern clause, got " + peek());
        }
    }
    return query;
}
