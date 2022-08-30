//
// Created by Xingchen Lin on 28/8/22.
//

#include "catch.hpp"

#include "query_builder/QueryBuilder.h"
#include "query_builder/QueryParser.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/FollowsClause.h"
#include "query_builder/clauses/FollowsTClause.h"

using QB::QueryParser;
using QB::QueryBuilder;
using QB::QueryTokenizer;
using QB::Synonym;
using QB::DesignEntity;
using QB::Declaration;
using QB::Clause;
using QB::SelectClause;
using QB::FollowsClause;
using QB::FollowsTClause;
using QB::Underscore;

TEST_CASE ("Test query Parser") {
    SECTION ("variable v1; Select v1") {
        std::string queryStr = "variable v1; Select v1";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v1"))});
        REQUIRE(query.clauses->size() == 1);
        SelectClause* clause = dynamic_cast<SelectClause*>(query.clauses->at(0));
        REQUIRE(*clause ==
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
        REQUIRE(query.clauses->size() == 1);
        SelectClause* clause = dynamic_cast<SelectClause*>(query.clauses->at(0));
        REQUIRE(*clause ==
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
        REQUIRE(query.clauses->size() == 1);
        SelectClause* clause = dynamic_cast<SelectClause*>(query.clauses->at(0));
        REQUIRE(*clause ==
                SelectClause(Synonym("a")));
    }

    SECTION ("stmt s; Select s such that Follows (6, s)") {
        std::string queryStr = "stmt s; Select s such that Follows (6, s)";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        REQUIRE(query.clauses->size() == 2);
        SelectClause* selectClause = dynamic_cast<SelectClause*>(query.clauses->at(0));
        REQUIRE(*selectClause ==
                SelectClause(Synonym("s")));
        FollowsClause* followsClause = dynamic_cast<FollowsClause*>(query.clauses->at(1));
        REQUIRE(*followsClause ==
                        FollowsClause(6, Synonym("s")));
    }

    SECTION ("stmt s; Select s such that Follows* (s, 6)") {
        std::string queryStr = "stmt s; Select s such that Follows* (s, 6)";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        REQUIRE(query.clauses->size() == 2);
        SelectClause* selectClause = dynamic_cast<SelectClause*>(query.clauses->at(0));
        REQUIRE(*selectClause ==
                SelectClause(Synonym("s")));
        FollowsTClause* followsTClause = dynamic_cast<FollowsTClause*>(query.clauses->at(1));
        REQUIRE(*followsTClause ==
                        FollowsTClause(Synonym("s"), 6));
    }

    SECTION ("stmt s; Select s such that Follows* (s, _)") {
        std::string queryStr = "stmt s; Select s such that Follows* (s, _)";
        auto query = QueryBuilder().buildPQLQuery(queryStr);
        REQUIRE(query.declarations->size() == 1);
        REQUIRE(*(query.declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        REQUIRE(query.clauses->size() == 2);
        SelectClause* selectClause = dynamic_cast<SelectClause*>(query.clauses->at(0));
        REQUIRE(*selectClause ==
                SelectClause(Synonym("s")));
        FollowsTClause* followsTClause = dynamic_cast<FollowsTClause*>(query.clauses->at(1));
        REQUIRE(*followsTClause ==
                FollowsTClause(Synonym("s"), Underscore()));
    }
}
