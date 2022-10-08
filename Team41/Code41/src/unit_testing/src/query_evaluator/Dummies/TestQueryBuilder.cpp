//
// Created by Nafour on 8/10/22.
//

#include "TestQueryBuilder.h"
#include "query_builder/clauses/such_that_clauses/FollowsClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesSClause.h"
#include "query_builder/clauses/such_that_clauses/NextClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/such_that_clauses/ParentClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsClause.h"
#include "query_builder/clauses/such_that_clauses/AffectsTClause.h"
#include "query_builder/clauses/such_that_clauses/NextTClause.h"
#include "query_builder/clauses/such_that_clauses/FollowsTClause.h"
#include "query_builder/clauses/such_that_clauses/ParentTClause.h"
#include "query_builder/clauses/such_that_clauses/ModifiesPClause.h"
#include "query_builder/clauses/such_that_clauses/UsesPClause.h"
#include "query_builder/clauses/such_that_clauses/UsesSClause.h"
#include "query_builder/clauses/such_that_clauses/CallsTClause.h"
#include "query_builder/clauses/such_that_clauses/CallsClause.h"

namespace TestQE {
    TestQueryBuilder::TestQueryBuilder() : declarations(make_shared<vector<Declaration>>()),
                                           suchThatClauses(make_shared<vector<shared_ptr<SuchThatClause>>>()),
                                           patternClauses(make_shared<vector<shared_ptr<PatternClause>>>()),
                                           withClauses(make_shared<vector<shared_ptr<WithClause>>>()) {
        //make default select type to be tuple
        this->selectClause = make_shared<SelectClause>(ReturnType::TUPLE);
        this->selectClause->returnResults = make_shared<vector<Elem>>();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addDeclaration(DesignEntity designEntity, Synonym synonym) {
        declarations->push_back(Declaration(designEntity, synonym));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::setReturnBoolean() {
        this->selectClause = make_shared<SelectClause>(ReturnType::BOOLEAN);
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::setReturnTuple() {
        this->selectClause = make_shared<SelectClause>(ReturnType::TUPLE);
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addToSelect(Elem elem) {
        this->selectClause->returnResults->push_back(elem);
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addAffects(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<AffectsClause>(arg1, arg2));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addAffectsT(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<AffectsTClause>(arg1, arg2));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addCalls(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<CallsClause>(arg1, arg2));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addCallT(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<CallsTClause>(arg1, arg2));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addFollows(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<FollowsClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addFollowsT(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<FollowsTClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addModifiesS(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<ModifiesSClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addModifiesP(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<ModifiesPClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addNext(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<NextClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addNextT(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<NextTClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addParent(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<ParentClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addParentT(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<ParentTClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addUsesS(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<UsesSClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addUsesP(Ref arg1, Ref arg2) {
        this->suchThatClauses->push_back(make_shared<UsesPClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addWith(WithRef arg1, WithRef arg2) {
        this->withClauses->push_back(make_shared<WithClause>(arg1, arg2));
        return shared_from_this();

    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addAssignPattern(Synonym arg1, Ref arg2, ExpressionSpec arg3) {
        this->patternClauses->push_back(make_shared<AssignPatternClause>(arg1, arg2, arg3));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addIfPattern(Synonym arg1, Ref arg2) {
        this->patternClauses->push_back(make_shared<IfPatternClause>(arg1, arg2));
        return shared_from_this();
    }

    shared_ptr<TestQueryBuilder> TestQueryBuilder::addWhilePattern(Synonym arg1, Ref arg2) {
        this->patternClauses->push_back(make_shared<WhilePatternClause>(arg1, arg2));
        return shared_from_this();
    }

    shared_ptr<Query> TestQueryBuilder::build() {
        auto query = make_shared<Query>();
        query->declarations = declarations;
        query->selectClause = selectClause;
        query->suchThatClauses = suchThatClauses;
        query->patternClauses = patternClauses;
        query->withClauses = withClauses;
        return query;
    }


} // TestQE