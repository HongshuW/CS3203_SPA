//
// Created by Nafour on 4/10/22.
//

#include "catch.hpp"
#include <memory>
#include "query_builder/commons/Query.h"
#include "query_evaluator/QueryOptimizer.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsTClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"

using namespace std;
TEST_CASE("Test query optimizer") {
    SECTION("Test building query optimizer with three groups") {
        shared_ptr<Query> query = make_shared<Query>();

        shared_ptr<SuchThatClause> suchThatClause1 = make_shared<ModifiesSClause>(Synonym("s1"), Synonym("v1"));
        shared_ptr<SuchThatClause> suchThatClause2 = make_shared<ModifiesSClause>(Synonym("s2"), Synonym("v1"));

        shared_ptr<SuchThatClause> suchThatClause3 = make_shared<ModifiesSClause>(Synonym("s3"), Synonym("v2"));
        shared_ptr<SuchThatClause> suchThatClause4 = make_shared<ModifiesSClause>(Underscore(), Synonym("v2"));

        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        query->suchThatClauses->push_back(suchThatClause3);
        query->suchThatClauses->push_back(suchThatClause4);

        shared_ptr<QE::QueryOptimizer> queryOptimizer = make_shared<QE::QueryOptimizer>(query);
        auto map = queryOptimizer->optimise();
        REQUIRE(map->size() == 3); //extra default group with no synonyms
    }
    SECTION("Test building query optimizer with no syn clauses") {
        shared_ptr<Query> query = make_shared<Query>();

        shared_ptr<SuchThatClause> suchThatClause1 = make_shared<ModifiesSClause>(Synonym("s1"), Synonym("v1"));
        shared_ptr<SuchThatClause> suchThatClause2 = make_shared<UsesSClause>(Synonym("s2"), Synonym("v1"));

        shared_ptr<SuchThatClause> suchThatClause3 = make_shared<FollowsClause>(Synonym("s3"), Synonym("s4"));
        shared_ptr<SuchThatClause> suchThatClause4 = make_shared<FollowsTClause>(Underscore(), Synonym("s3"));

        shared_ptr<SuchThatClause> suchThatClause5 = make_shared<ModifiesSClause>(Underscore(), Ident("haha1"));
        shared_ptr<SuchThatClause> suchThatClause6 = make_shared<ModifiesSClause>( Ident("haha2"), Underscore());

        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        query->suchThatClauses->push_back(suchThatClause3);
        query->suchThatClauses->push_back(suchThatClause4);
        query->suchThatClauses->push_back(suchThatClause5);
        query->suchThatClauses->push_back(suchThatClause6);

        shared_ptr<QE::QueryOptimizer> queryOptimizer = make_shared<QE::QueryOptimizer>(query);
        auto map = queryOptimizer->optimise();
        REQUIRE(map->size() == 3);
    }
    SECTION("Test building query optimizer with no syn clauses and different clause types") {
        shared_ptr<Query> query = make_shared<Query>();

        shared_ptr<SuchThatClause> suchThatClause1 = make_shared<ModifiesSClause>(Synonym("s1"), Synonym("v1"));
        shared_ptr<WithClause> withClause1 = make_shared<WithClause>(AttrRef(Synonym("s2"), QB::AttrName::STMT_NUMBER),
                                                                         AttrRef(Synonym("v1"), QB::AttrName::VAR_NAME));

        shared_ptr<SuchThatClause> suchThatClause3 = make_shared<FollowsClause>(Synonym("s3"), Synonym("s4"));
        shared_ptr<PatternClause> patternClause1 = make_shared<AssignPatternClause>(Synonym("s3"), Underscore());

        shared_ptr<SuchThatClause> suchThatClause5 = make_shared<ModifiesSClause>(Underscore(), Ident("haha1"));
        shared_ptr<SuchThatClause> suchThatClause6 = make_shared<ModifiesSClause>( Ident("haha2"), Underscore());

        query->suchThatClauses->push_back(suchThatClause1);
        query->withClauses->push_back(withClause1);
        query->suchThatClauses->push_back(suchThatClause3);
        query->patternClauses->push_back(patternClause1);
        query->suchThatClauses->push_back(suchThatClause5);
        query->suchThatClauses->push_back(suchThatClause6);

        shared_ptr<QE::QueryOptimizer> queryOptimizer = make_shared<QE::QueryOptimizer>(query);
        auto map = queryOptimizer->optimise();
        REQUIRE(map->size() == 3);
    }
    SECTION("Test building query optimizer sorting with two groups") {
        shared_ptr<Query> query = make_shared<Query>();
        shared_ptr<SuchThatClause> suchThatClause1 = make_shared<ModifiesSClause>(Synonym("s1"), Synonym("v1"));
        shared_ptr<SuchThatClause> suchThatClause2 = make_shared<UsesSClause>(Underscore(), Synonym("v1"));
        shared_ptr<SuchThatClause> suchThatClause3 = make_shared<FollowsClause>(Synonym("s1"), Synonym("s4"));
        shared_ptr<SuchThatClause> suchThatClause4 = make_shared<FollowsTClause>(Underscore(), Synonym("s3"));
        shared_ptr<SuchThatClause> suchThatClause5 = make_shared<FollowsTClause>(Synonym("s5"), Synonym("s3"));
        shared_ptr<SuchThatClause> suchThatClause6 = make_shared<ModifiesPClause>(Underscore(), Synonym("v1"));
        shared_ptr<SuchThatClause> suchThatClause7 = make_shared<FollowsTClause>(Synonym("s4"), Synonym("s5"));

        query->suchThatClauses->push_back(suchThatClause1);
        query->suchThatClauses->push_back(suchThatClause2);
        query->suchThatClauses->push_back(suchThatClause3);
        query->suchThatClauses->push_back(suchThatClause4);
        query->suchThatClauses->push_back(suchThatClause5);
        query->suchThatClauses->push_back(suchThatClause6);
        query->suchThatClauses->push_back(suchThatClause7);

        shared_ptr<QE::QueryOptimizer> queryOptimizer = make_shared<QE::QueryOptimizer>(query);
        auto map = queryOptimizer->optimise();

        REQUIRE(map->size() == 2);

        const int ONE_SYN_CLAUSE_COUNT = 3;

        for (auto it: *map) {
            if (it.first != QE::QueryOptimizer::NO_SYN_GROUP_IDX) {
                for (int i = 0; i < it.second->size(); i++) {
                    if (i < ONE_SYN_CLAUSE_COUNT) {
                        REQUIRE(it.second->at(i)->getSynonymNames().size() == 1);
                    } else {
                        REQUIRE(it.second->at(i)->getSynonymNames().size() == 2);
                    }
                }
            }
        }

    }
}
