//
// Created by Xingchen Lin on 28/8/22.
//

#include "catch.hpp"

#include "query_builder/QueryBuilder.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/exceptions/Exceptions.h"

using namespace QB;

TEST_CASE ("Test Query Parser") {
//    SECTION ("variable v1; Select v1") {
//        std::string queryStr = "variable v1; Select v1";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::VARIABLE, Synonym("v1"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("v1")));
//    }
//
//    SECTION ("stmt a, b, c; Select a") {
//        std::string queryStr = "stmt a, b, c; Select a";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 3);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::STMT, Synonym("a")),
//                        Declaration(DesignEntity::STMT, Synonym("b")),
//                        Declaration(DesignEntity::STMT, Synonym("c"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//    }
//
//    SECTION ("assign a; while w; Select a") {
//        std::string queryStr = "assign a; while w; Select a";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 2);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
//                        Declaration(DesignEntity::WHILE, Synonym("w"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//    }
//
//    SECTION ("stmt s; Select s such that Parent (s, 12)") {
//        std::string queryStr = "stmt s; Select s such that Parent (s, 12)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::STMT, Synonym("s"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("s")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::PARENT, Synonym("s"), 12,
//                               query->declarations));
//    }
//
//    SECTION ("assign a; Select a such that Parent* (_, a)") {
//        std::string queryStr = "assign a; Select a such that Parent* (_, a)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::PARENT_T, Underscore(), Synonym("a"),
//                               query->declarations));
//    }
//
//    SECTION ("stmt s; Select s such that Follows (6, s)") {
//        std::string queryStr = "stmt s; Select s such that Follows (6, s)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::STMT, Synonym("s"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("s")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::FOLLOWS, 6, Synonym("s"),
//                               query->declarations));
//    }
//
//    SECTION ("stmt s; Select s such that Follows* (s, 6)") {
//        std::string queryStr = "stmt s; Select s such that Follows* (s, 6)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::STMT, Synonym("s"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("s")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::FOLLOWS_T, Synonym("s"), 6,
//                               query->declarations));
//    }
//
//    SECTION ("stmt s; Select s such that Follows* (s, _)") {
//        std::string queryStr = "stmt s; Select s such that Follows* (s, _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::STMT, Synonym("s"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("s")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::FOLLOWS_T, Synonym("s"), Underscore(),
//                               query->declarations));
//    }
//
//    SECTION ("assign a; Select a such that Uses (a, \"x\")") {
//        std::string queryStr = "assign a; Select a such that Uses (a, \"x\")";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::USES_P, Synonym("a"), Ident("x"),
//                               query->declarations));
//    }
//
//    SECTION ("variable v; Select v such that Modifies (\"main\", v)") {
//        std::string queryStr = "variable v; Select v such that Modifies (\"main\", v)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("v")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(query->suchThatClauses->at(0)->relationType == RelationType::MODIFIES_P);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::MODIFIES_P, Ident("main"), Synonym("v"),
//                               query->declarations));
//    }
//
//    SECTION ("variable v; Select v such that Modifies (8, v)") {
//        std::string queryStr = "variable v; Select v such that Modifies (8, v)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("v")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(query->suchThatClauses->at(0)->relationType == RelationType::MODIFIES_S);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::MODIFIES_S, 8, Synonym("v"),
//                               query->declarations));
//    }
//
//    SECTION ("stmt s; Select s such that Modifies (s, _)") {
//        std::string queryStr = "stmt s; Select s such that Modifies (s, _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::STMT, Synonym("s"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("s")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(query->suchThatClauses->at(0)->relationType == RelationType::MODIFIES_S);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::MODIFIES_S, Synonym("s"), Underscore(),
//                               query->declarations));
//    }
//
//    SECTION ("procedure p; Select p such that Modifies (p, _)") {
//        std::string queryStr = "procedure p; Select p such that Modifies (p, _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("p")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(query->suchThatClauses->at(0)->relationType == RelationType::MODIFIES_P);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::MODIFIES_P, Synonym("p"), Underscore(),
//                               query->declarations));
//    }
//
//    SECTION ("assign a; Select a such that Uses (a, _)") {
//        std::string queryStr = "assign a; Select a such that Uses (a, _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(query->suchThatClauses->at(0)->relationType == RelationType::USES_S);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::USES_P, Synonym("a"), Underscore(),
//                               query->declarations));
//    }
//
//    SECTION ("procedure p; Select p such that Uses (p, _)") {
//        std::string queryStr = "procedure p; Select p such that Uses (p, _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("p")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(query->suchThatClauses->at(0)->relationType == RelationType::USES_P);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::USES_P, Synonym("p"), Underscore(),
//                               query->declarations));
//    }
//
//    SECTION ("assign a; Select a pattern a (\"test\", _)") {
//        std::string queryStr = "assign a; Select a pattern a (\"test\", _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//        REQUIRE(*(query->patternClause) ==
//                PatternClause(
//                        Synonym("a"),
//                        Ident("test"),
//                        ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
//    }
//
//    SECTION ("assign a; Select a pattern a (_, _)") {
//        std::string queryStr = "assign a; Select a pattern a (_, _)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 1);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//        REQUIRE(*(query->patternClause) ==
//                PatternClause(
//                        Synonym("a"),
//                        Underscore(),
//                        ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
//    }

    SECTION ("assign a; Select a pattern a (_, \"x + 1\")") {
        std::string queryStr = "assign a; Select a pattern a (_, \"x + 1\")";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        REQUIRE(*(query->selectClause) ==
                SelectClause(Synonym("a")));
        shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("+");
        exprNode->left = make_shared<ExprNode>("x");
        exprNode->right = make_shared<ExprNode>("1");
        REQUIRE(*(query->patternClause) ==
                PatternClause(
                        Synonym("a"),
                        Underscore(),
                        ExpressionSpec(ExpressionSpecType::FULL_MATCH,
                                       exprNode)));
    }
//
//    SECTION ("assign a; variable v; Select a such that Uses (a, \"x\") pattern a (v, _\"y\"_)") {
//        std::string queryStr = "assign a; variable v; Select a such that Uses (a, \"x\") pattern a (v, _\"y\"_)";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 2);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
//                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("a")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::USES_P, Synonym("a"), Ident("x"),
//                               query->declarations));
//        REQUIRE(*(query->patternClause) ==
//                PatternClause(
//                        Synonym("a"),
//                        Synonym("v"),
//                        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
//                                       make_shared<ExprNode>("y"))));
//    }
//
//    SECTION ("assign a; variable v; Select v pattern a (v, _\"y\"_) such that Uses (a, \"x\")") {
//        std::string queryStr = "assign a; variable v; Select v pattern a (v, _\"y\"_) such that Uses (a, \"x\")";
//        auto query = QueryBuilder().buildPQLQuery(queryStr);
//        REQUIRE(query->declarations->size() == 2);
//        REQUIRE(*(query->declarations) ==
//                std::vector<Declaration>{
//                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
//                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
//        REQUIRE(*(query->selectClause) ==
//                SelectClause(Synonym("v")));
//        REQUIRE(query->suchThatClauses->size() == 1);
//        REQUIRE(*(query->suchThatClauses)->at(0) ==
//                SuchThatClause(RelationType::USES_P, Synonym("a"), Ident("x"),
//                               query->declarations));
//        REQUIRE(*(query->patternClause) ==
//                PatternClause(
//                        Synonym("a"),
//                        Synonym("v"),
//                        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
//                                       make_shared<ExprNode>("y"))));
//    }
//
//    SECTION ("'select' is not defined, throw PQLParseException") {
//        std::string queryStr = "variable v; select v";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Unexpected token ';', throw PQLParseException") {
//        std::string queryStr = "variable v; Select v;";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("'uses' is not defined, throw PQLParseException") {
//        std::string queryStr = "variable v; Select v such that uses (1, v)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("'such that' is not found, throw PQLParseException") {
//        std::string queryStr = "variable v; Select v Uses (1, v)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("no ';' after declaration, throw PQLParseException") {
//        std::string queryStr = "variable v Select v Uses (1, v)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("no ';' after declaration, found ',', throw PQLParseException") {
//        std::string queryStr = "variable v, Select v Uses (1, v)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Invalid synonym in Ref arg, throw PQLParseException") {
//        std::string queryStr = "variable 123abc;";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Invalid synonym declared, throw PQLParseException") {
//        std::string queryStr = "variable v, Select v Uses (2, 123abc)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Invalid Ref, throw PQLParseException") {
//        std::string queryStr = "variable v, Select v Uses ((,), v)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Empty arg 2 in pattern clause, throw PQLParseException") {
//        std::string queryStr = "assign a, Select a pattern a (, _)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("'Pattern' is not defined, throw PQLParseException") {
//        std::string queryStr = "assign a, Select a Pattern a (_, _)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Invalid expression (number), throw PQLParseException") {
//        std::string queryStr = "assign a, Select a pattern a (_, 123)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
//
//    SECTION ("Invalid expression (synonym), throw PQLParseException") {
//        std::string queryStr = "assign a, Select a pattern a (_, x)";
//        auto queryBuilder = QueryBuilder();
//        REQUIRE_THROWS_AS(queryBuilder.buildPQLQuery(queryStr), PQLParseException);
//    }
}