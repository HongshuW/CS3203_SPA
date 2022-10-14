//
// Created by Xingchen Lin on 28/8/22.
//

#include "query_builder/QueryParser.h"

#include <utility>
#include "query_builder/commons/Synonym.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "query_builder/clauses/such_that_clauses/SuchThatClause.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

namespace QB {
}

QueryParser::QueryParser(std::vector<std::string> tokens)
        : query(new Query()), currIdx(QueryParserConstants::ZERO), tokens(std::move(tokens)) {}

string QueryParser::peek() { return tokens[currIdx]; }

string QueryParser::pop() {
    string currToken = tokens[currIdx];
    currIdx++;
    return currToken;
}

bool QueryParser::isWithinBound() {
    return currIdx < tokens.size();
}

bool QueryParser::match(const string& s) {
    if (isWithinBound() && peek() == s) {
        currIdx++;
        return true;
    }
    return false;
}

bool QueryParser::expect(const string& s) {
    if (match(s)) {
        return true;
    }
    string errorMessage = ErrorMessageFormatter::formatErrorMessage(s, peek());
    throw PQLParseException(errorMessage);
}

bool QueryParser::parseDeclarations() {
    int savedIdx = currIdx;
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
    while(!match(QueryParserConstants::SEMICOLON)) {
        expect(QueryParserConstants::COMMA);
        synonymStr = pop();
        synonym = Synonym(synonymStr);
        synonymList.push_back(synonym);
    }

    for (const Synonym& syn : synonymList) {
        Declaration declaration = Declaration(designEntity, syn);
        query->declarations->push_back(declaration);
    }

    return true;
}

void QueryParser::parseBooleanSelectClause() {
    //! If BOOLEAN is declared as a synonym in a PQL query, this declaration takes precedence
    shared_ptr<SelectClause> selectClause;
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    Synonym syn = Synonym(QueryParserConstants::BOOLEAN);
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
    if (match(QueryParserConstants::FULL_STOP)) {
        string attrNameStr = pop();
        AttrName attrName = AttrRef::getAttrNameFromStr(attrNameStr);
        AttrRef attrRef = AttrRef(synonym, attrName);
        return attrRef;
    } else {
        return synonym;
    }
}

void QueryParser::parseSelectClause() {
    //! For BOOLEAN
    if (match(QueryParserConstants::BOOLEAN)) {
        parseBooleanSelectClause();
        return;
    }

    shared_ptr<SelectClause> selectClause;
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    //! For Tuple
    if (match(QueryParserConstants::SMALLER_THAN)) {
        while (!match(QueryParserConstants::GREATER_THAN)) {
            Elem elem = parseTupleSelectClause();
            returnResults->push_back(elem);
            selectClause = make_shared<SelectClause>(ReturnType::TUPLE, returnResults);
            query->selectClause = selectClause;
            if (match(QueryParserConstants::GREATER_THAN)) return;
            expect(QueryParserConstants::COMMA);
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
    if (match(QueryParserConstants::UNDERSCORE)) {
        return Underscore();
    } else if (match(QueryParserConstants::DOUBLE_QUOTE)) {
        string identStr = pop();
        expect(QueryParserConstants::DOUBLE_QUOTE);
        Ident ident = Ident(identStr);
        return ident;
    } else if (Utils::isValidNumber(peek())) {
        return std::stoi(pop());
    } else if (Utils::isValidName(peek())) {
        string synonymStr = pop();
        Synonym synonym = Synonym(synonymStr);
        return synonym;
    } else {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_REF, peek());
        throw PQLParseException(errorMessage);
    }
}

void QueryParser::parseSuchThatClause() {
    string relationTypeStr = pop();
    RelationType relationType = getRelationTypeFromStr(relationTypeStr);

    expect(QueryParserConstants::LEFT_BRACKET);
    auto arg1 = parseRef();
    expect(QueryParserConstants::COMMA);
    auto arg2 = parseRef();
    expect(QueryParserConstants::RIGHT_BRACKET);
    const unordered_map<RelationType, shared_ptr<SuchThatRelations>> SUCH_THAT_CREATORS({
        {RelationType::FOLLOWS, make_shared<Follows>()},
        {RelationType::FOLLOWS_T, make_shared<FollowsT>()},
        {RelationType::PARENT, make_shared<Parent>()},
        {RelationType::PARENT_T, make_shared<ParentT>()},
        {RelationType::USES, make_shared<Uses>(query->declarations)},
        {RelationType::MODIFIES, make_shared<Modifies>(query->declarations)},
        {RelationType::CALLS, make_shared<Calls>()},
        {RelationType::CALLS_T, make_shared<CallsT>()},
        {RelationType::NEXT, make_shared<Next>()},
        {RelationType::NEXT_T, make_shared<NextT>()},
        {RelationType::AFFECTS, make_shared<Affects>()},
        {RelationType::AFFECTS_T, make_shared<AffectsT>()}
    });
    const auto& suchThatClauseCreator = SUCH_THAT_CREATORS.at(relationType);
    auto suchThatClause = suchThatClauseCreator->createClause(arg1, arg2);
    query->suchThatClauses->push_back(dynamic_pointer_cast<SuchThatClause>(suchThatClause));
}

bool QueryParser::parseSuchThat() {
    if (!match(QueryParserConstants::SUCH)) return false;
    expect(QueryParserConstants::THAT);
    parseSuchThatClause();
    while(match(QueryParserConstants::AND)) {
        parseSuchThatClause();
    }
    return true;
}

ExpressionSpec QueryParser::parseExpressionSpec() {
    vector<string> expr;
    if (match(QueryParserConstants::DOUBLE_QUOTE)) {
        //! Full match
        ExprStringTokenizer tokenizer = ExprStringTokenizer(pop());
        expr = tokenizer.tokenize();
        if (expr.empty()) {
            throw PQLParseException(QueryParserConstants::PQL_PARSE_EXCEPTION_EMPTY_ASSIGN_EXPRESSION);
        }
        expect(QueryParserConstants::DOUBLE_QUOTE);
        ExprNodeParser parser = ExprNodeParser(expr);
        return {ExpressionSpecType::FULL_MATCH, parser.parse()};
    } else if (match(QueryParserConstants::UNDERSCORE)) {
        if (peek() == QueryParserConstants::RIGHT_BRACKET) {
            //! Any match
            return {ExpressionSpecType::ANY_MATCH};
        } else {
            //! Partial match
            expect(QueryParserConstants::DOUBLE_QUOTE);
            ExprStringTokenizer tokenizer = ExprStringTokenizer(pop());
            expr = tokenizer.tokenize();
            if (expr.empty()) {
                throw PQLParseException(QueryParserConstants::PQL_PARSE_EXCEPTION_EMPTY_ASSIGN_EXPRESSION);
            }
            expect(QueryParserConstants::DOUBLE_QUOTE);
            expect(QueryParserConstants::UNDERSCORE);
            ExprNodeParser parser = ExprNodeParser(expr);
            return {ExpressionSpecType::PARTIAL_MATCH, parser.parse()};
        }
    } else {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_EXPRESSION, peek());
        throw PQLParseException(errorMessage);
    }
}

void QueryParser::parsePatternClause() {
    Synonym arg1 = Synonym(pop());
    //! Default value
    DesignEntity de = DesignEntity::STMT;
    auto declaration = Declaration::findDeclaration(arg1, query->declarations);
    expect(QueryParserConstants::LEFT_BRACKET);
    if (declaration) {
        de = declaration->getDesignEntity();
    }
    Ref arg2 = parseRef();
    expect(QueryParserConstants::COMMA);
    const unordered_set<DesignEntity> ALLOWED_DE = {
            DesignEntity::IF, DesignEntity::WHILE, DesignEntity::ASSIGN
    };
    const unordered_map<DesignEntity, shared_ptr<PatternRelations>> PATTERN_CREATORS({
        {DesignEntity::IF, make_shared<IfPattern>()},
        {DesignEntity::WHILE, make_shared<WhilePattern>()},
        {DesignEntity::ASSIGN, make_shared<AssignPattern>()},
    });

    //! Default patternClauseCreator, will change later
    shared_ptr<PatternRelations> patternClauseCreator = make_shared<PatternRelations>();
    if (ALLOWED_DE.count(de)) {
        patternClauseCreator = PATTERN_CREATORS.at(de);
    } else {
        //! Throw error in Validator
        patternClauseCreator = make_shared<InvalidPattern>();
    }

    auto patternClause = dynamic_pointer_cast<PatternClause>(patternClauseCreator->createClause(arg1, arg2));
    currIdx = patternClause->validateSyntaxError(currIdx, tokens);
    //! Special case, need to parse ExpressionSpec
    if (dynamic_pointer_cast<AssignPatternClause>(patternClause)) {
        ExpressionSpec arg3 = parseExpressionSpec();
        dynamic_pointer_cast<AssignPatternClause>(patternClause)->arg3 = arg3;
    }
    expect(QueryParserConstants::RIGHT_BRACKET);
    query->patternClauses->push_back(patternClause);
}

bool QueryParser::parsePattern() {
    if (!match(QueryParserConstants::PATTERN)) return false;
    parsePatternClause();
    while(match(QueryParserConstants::AND)) {
        parsePatternClause();
    }
    return true;
}

WithRef QueryParser::parseWithRef() {
    //! Can be ident, integer, or AttrRef
    if (Utils::isValidNumber(peek())) {
        return std::stoi(pop());
    } else if (match(QueryParserConstants::DOUBLE_QUOTE)) {
        string identStr = pop();
        expect(QueryParserConstants::DOUBLE_QUOTE);
        Ident ident = Ident(identStr);
        return ident;
    } else if (Utils::isValidName(peek())) {
        string synonymStr = pop();
        Synonym synonym = Synonym(synonymStr);
        expect(QueryParserConstants::FULL_STOP);
        string attrNameStr = pop();
        AttrName attrName = AttrRef::getAttrNameFromStr(attrNameStr);
        AttrRef attrRef = AttrRef(synonym, attrName);
        return attrRef;
    } else {
        string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_WITH_REF, peek());
        throw PQLParseException(errorMessage);
    }
}

void QueryParser::parseWithClause() {
    WithRef lhs = parseWithRef();
    expect(QueryParserConstants::EQUAL);
    WithRef rhs = parseWithRef();
    shared_ptr<WithClause> withClause = make_shared<WithClause>(lhs, rhs);
    query->withClauses->push_back(withClause);
}

bool QueryParser::parseWith() {
    if (!match(QueryParserConstants::WITH)) return false;
    parseWithClause();
    while(match(QueryParserConstants::AND)) {
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
        expect(QueryParserConstants::SELECT);

        parseSelectClause();

        while (isWithinBound()) {
            if (parseSuchThat()) continue;
            if (parsePattern()) continue;
            if (parseWith()) continue;
            string errorMessage = ErrorMessageFormatter::formatErrorMessage(
                    QueryParserConstants::PQL_PARSE_EXCEPTION_EXPECT_SUCH_THAT_OR_PATTERN, peek());
            throw PQLParseException(errorMessage);
        }
    }
    return query;
}
