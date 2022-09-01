//
// Created by Xingchen Lin on 28/8/22.
//

#include "catch.hpp"

#include "query_builder/QueryBuilder.h"
#include "query_builder/QueryParser.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_builder/commons/Ref.h"

using QB::QueryParser;
using QB::QueryBuilder;
using QB::QueryTokenizer;
using QB::Synonym;
using QB::DesignEntity;
using QB::Declaration;
using QB::SelectClause;
using QB::SuchThatClause;
using QB::Underscore;

TEST_CASE ("Test Query Parser") {
    SECTION ("variable v1; Select v1") {
        std::string queryStr = "variable v1; Select v1";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v1"))});
        REQUIRE(*(query.selectClause) ==
                SelectClause(Synonym("v1")));
    }

    SECTION ("stmt a, b, c; Select a") {
        std::string queryStr = "stmt a, b, c; Select a";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 3);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("a")),
                        Declaration(DesignEntity::STMT, Synonym("b")),
                        Declaration(DesignEntity::STMT, Synonym("c"))});
        REQUIRE(*(query.selectClause) ==
                SelectClause(Synonym("a")));
    }

    SECTION ("assign a; while w; Select a") {
        std::string queryStr = "assign a; while w; Select a";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 2);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::WHILE, Synonym("w"))});
        REQUIRE(*(query.selectClause) ==
                SelectClause(Synonym("a")));
    }

    SECTION ("stmt s; Select s such that Follows (6, s)") {
        std::string queryStr = "stmt s; Select s such that Follows (6, s)";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        REQUIRE(*(query.selectClause) ==
                SelectClause(Synonym("s")));
        REQUIRE(query.suchThatClauses->size() == 1);
        REQUIRE(*(query.suchThatClauses)->at(0) ==
                SuchThatClause(RelationType::FOLLOWS, 6, Synonym("s")));
    }

    SECTION ("stmt s; Select s such that Follows* (s, 6)") {
        std::string queryStr = "stmt s; Select s such that Follows* (s, 6)";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        REQUIRE(*(query.selectClause) ==
                SelectClause(Synonym("s")));
        REQUIRE(query.suchThatClauses->size() == 1);
        REQUIRE(*(query.suchThatClauses)->at(0) ==
                SuchThatClause(RelationType::FOLLOWS_T, Synonym("s"), 6));
    }

    SECTION ("stmt s; Select s such that Follows* (s, _)") {
        std::string queryStr = "stmt s; Select s such that Follows* (s, _)";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        REQUIRE(*(query.selectClause) ==
                SelectClause(Synonym("s")));
        REQUIRE(query.suchThatClauses->size() == 1);
        REQUIRE(*(query.suchThatClauses)->at(0) ==
                SuchThatClause(RelationType::FOLLOWS_T, Synonym("s"), Underscore()));
    }
}
