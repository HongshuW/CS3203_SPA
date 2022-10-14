//
// Created by Xingchen Lin on 28/8/22.
//

#include "catch.hpp"

#include "query_builder/QueryBuilder.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "query_builder/relations/such_that_relations/Follows.h"
#include "query_builder/relations/such_that_relations/FollowsT.h"
#include "query_builder/relations/such_that_relations/Parent.h"
#include "query_builder/relations/such_that_relations/ParentT.h"
#include "query_builder/relations/such_that_relations/UsesP.h"
#include "query_builder/relations/such_that_relations/UsesS.h"
#include "query_builder/relations/such_that_relations/ModifiesP.h"
#include "query_builder/relations/such_that_relations/ModifiesS.h"
#include "query_builder/relations/such_that_relations/Calls.h"
#include "query_builder/relations/such_that_relations/CallsT.h"
#include "query_builder/relations/such_that_relations/Next.h"
#include "query_builder/relations/such_that_relations/NextT.h"
#include "query_builder/relations/such_that_relations/Affects.h"
#include "query_builder/relations/such_that_relations/AffectsT.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/exceptions/Exceptions.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include <memory>
#include <vector>
#include <string>

using namespace QB;

shared_ptr<QueryBuilder> queryBuilder = make_shared<QueryBuilder>();

TEST_CASE ("Test Query Parser") {
    SECTION ("variable v1; Select v1") {
        std::string queryStr = "variable v1; Select v1";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v1"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v1"));
        REQUIRE(*(query->selectClause) == SelectClause(ReturnType::TUPLE, returnResults));
    }

    SECTION ("stmt a, b, c; Select a") {
        std::string queryStr = "stmt a, b, c; Select a";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 3);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("a")),
                        Declaration(DesignEntity::STMT, Synonym("b")),
                        Declaration(DesignEntity::STMT, Synonym("c"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) == SelectClause(ReturnType::TUPLE, returnResults));
    }

    SECTION ("assign a; while w; Select a") {
        std::string queryStr = "assign a; while w; Select a";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::WHILE, Synonym("w"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) == SelectClause(ReturnType::TUPLE, returnResults));
    }

    SECTION ("stmt s; Select s such that Parent (s, 12)") {
        std::string queryStr = "stmt s; Select s such that Parent (s, 12)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("s"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<ParentClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ParentClause(Synonym("s"), 12));
    }

    SECTION ("assign a; Select a such that Parent* (_, a)") {
        std::string queryStr = "assign a; Select a such that Parent* (_, a)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<ParentTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ParentTClause(Underscore(), Synonym("a")));
    }

    SECTION ("stmt s; Select s such that Follows (6, s)") {
        std::string queryStr = "stmt s; Select s such that Follows (6, s)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("s"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<FollowsClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == FollowsClause(6, Synonym("s")));
    }

    SECTION ("stmt s; Select s such that Follows* (s, 6)") {
        std::string queryStr = "stmt s; Select s such that Follows* (s, 6)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("s"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<FollowsTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == FollowsTClause(Synonym("s"), 6));
    }

    SECTION ("stmt s; Select s such that Follows* (s, _)") {
        std::string queryStr = "stmt s; Select s such that Follows* (s, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("s"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<FollowsTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == FollowsTClause(Synonym("s"), Underscore()));
    }

    SECTION ("assign a; Select a such that Uses (a, \"x\")") {
        std::string queryStr = "assign a; Select a such that Uses (a, \"x\")";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == UsesSClause(Synonym("a"), Ident("x")));
    }

    SECTION ("variable v; Select v such that Modifies (\"main\", v)") {
        std::string queryStr = "variable v; Select v such that Modifies (\"main\", v)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<ModifiesPClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ModifiesPClause(Ident("main"), Synonym("v")));
    }

    SECTION ("variable v; Select v such that Modifies (8, v)") {
        std::string queryStr = "variable v; Select v such that Modifies (8, v)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<ModifiesSClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ModifiesSClause(8, Synonym("v")));
    }

    SECTION ("stmt s; Select s such that Modifies (s, _)") {
        std::string queryStr = "stmt s; Select s such that Modifies (s, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("s"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<ModifiesSClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ModifiesSClause(Synonym("s"), Underscore()));
    }

    SECTION ("procedure p; Select p such that Modifies (p, _)") {
        std::string queryStr = "procedure p; Select p such that Modifies (p, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<ModifiesPClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ModifiesPClause(Synonym("p"), Underscore()));
    }

    SECTION ("assign a; Select a such that Uses (a, _)") {
        std::string queryStr = "assign a; Select a such that Uses (a, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == UsesSClause(Synonym("a"), Underscore()));
    }

    SECTION ("procedure p; Select p such that Uses (p, _)") {
        std::string queryStr = "procedure p; Select p such that Uses (p, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<UsesPClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == UsesPClause(Synonym("p"), Underscore()));
    }

    SECTION ("procedure p; Select p such that Calls* (p, _)") {
        std::string queryStr = "procedure p; Select p such that Calls* (p, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<CallsTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == CallsTClause(Synonym("p"), Underscore()));
    }

    SECTION ("while w; Select w such that Next (w, 2)") {
        std::string queryStr = "while w; Select w such that Next (w, 2)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::WHILE, Synonym("w"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("w"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<NextClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == NextClause(Synonym("w"), 2));
    }

    SECTION ("assign w; Select w such that Affects* (_, w)") {
        std::string queryStr = "assign w; Select w such that Affects* (_, w)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("w"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("w"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<AffectsTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == AffectsTClause(Underscore(), Synonym("w")));
    }

    SECTION ("assign a; Select a pattern a (\"test\", _)") {
        std::string queryStr = "assign a; Select a pattern a (\"test\", _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause ==
                AssignPatternClause(
                        Synonym("a"),
                        Ident("test"),
                        ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
    }

    SECTION ("assign a; Select a pattern a (_, _)") {
        std::string queryStr = "assign a; Select a pattern a (_, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause ==  AssignPatternClause(
                        Synonym("a"),
                        Underscore(),
                        ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
    }

    SECTION ("assign a; Select a pattern a (_, _) and pattern a (_, \"x + 1\")") {
        std::string queryStr = "assign a; Select a pattern a (_, _) and a (_, \"x + 1\")";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause ==  AssignPatternClause(
                Synonym("a"),
                Underscore(),
                ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
        AssignPatternClause assignPatternClause2 = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(1)));
        shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("+");
        exprNode->left = make_shared<ExprNode>("x");
        exprNode->right = make_shared<ExprNode>("1");
        REQUIRE(assignPatternClause2 ==  AssignPatternClause(
                Synonym("a"),
                Underscore(),
                ExpressionSpec(ExpressionSpecType::FULL_MATCH,
                               exprNode)));
    }

    SECTION ("assign a; variable x;  Select a pattern a (_, \"x + 1\")") {
        std::string queryStr = "assign a; variable x; Select a pattern a (_, \"x + 1\")";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::VARIABLE, Synonym("x"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("+");
        exprNode->left = make_shared<ExprNode>("x");
        exprNode->right = make_shared<ExprNode>("1");
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause == AssignPatternClause(
                        Synonym("a"),
                        Underscore(),
                        ExpressionSpec(ExpressionSpecType::FULL_MATCH,
                                       exprNode)));
    }

    SECTION ("assign a; Select a pattern a (_, \"(2 + 1)\")") {
        std::string queryStr = "assign a; Select a pattern a (_, \"(2 + 1)\")";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("+");
        exprNode->left = make_shared<ExprNode>("2");
        exprNode->right = make_shared<ExprNode>("1");
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause == AssignPatternClause(
                        Synonym("a"),
                        Underscore(),
                        ExpressionSpec(ExpressionSpecType::FULL_MATCH,
                                       exprNode)));
    }

    SECTION ("assign a; variable x; Select a pattern a (_, _\"(x + 1) * 3\"_)") {
        std::string queryStr = "assign a; variable x; Select a pattern a (_, _\"(x + 1) * 3\"_)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::VARIABLE, Synonym("x"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        shared_ptr<ExprNode> exprNodeLeft = make_shared<ExprNode>("+");
        exprNodeLeft->left = make_shared<ExprNode>("x");
        exprNodeLeft->right = make_shared<ExprNode>("1");
        shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("*");
        exprNode->left = exprNodeLeft;
        exprNode->right = make_shared<ExprNode>("3");
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause == AssignPatternClause(
                        Synonym("a"),
                        Underscore(),
                        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
                                       exprNode)));
    }

    SECTION ("assign a; variable v, y; Select a such that Uses (a, \"x\") pattern a (v, _\"y\"_)") {
        std::string queryStr = "assign a; variable v, y; Select a such that Uses (a, \"x\") pattern a (v, _\"y\"_)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 3);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v")),
                        Declaration(DesignEntity::VARIABLE, Synonym("y"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == UsesSClause(Synonym("a"), Ident("x")));
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause ==
                AssignPatternClause(
                        Synonym("a"),
                        Synonym("v"),
                        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
                                       make_shared<ExprNode>("y"))));
    }

    SECTION ("assign a; variable v; Select v pattern a (v, _\"y\"_) such that Uses (a, \"x\")") {
        std::string queryStr = "assign a; variable v; Select v pattern a (v, _\"y\"_) such that Uses (a, \"x\")";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == UsesSClause(Synonym("a"), Ident("x")));
        AssignPatternClause assignPatternClause = dynamic_cast<AssignPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(assignPatternClause ==  AssignPatternClause(
                        Synonym("a"),
                        Synonym("v"),
                        ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
                                       make_shared<ExprNode>("y"))));
    }

    SECTION ("if i; variable v; Select v pattern i (v, _, _)") {
        std::string queryStr = "if i; variable v; Select v pattern i (v, _, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::IF, Synonym("i")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        IfPatternClause ifPatternClause = dynamic_cast<IfPatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(ifPatternClause == IfPatternClause(
                Synonym("i"),
                Synonym("v")));
    }

    SECTION ("variable v; Select v such that Calls (_, \"testProcedure\")") {
        std::string queryStr = "variable v; Select v such that Calls (_, \"testProcedure\")";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        auto clause = dynamic_pointer_cast<CallsClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == CallsClause(Underscore(), Ident("testProcedure")));
    }

    SECTION ("variable v; Select v such that Calls* (_, _)") {
        std::string queryStr = "variable v; Select v such that Calls* (_, _)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("v"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        auto clause = dynamic_pointer_cast<CallsTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == CallsTClause(Underscore(), Underscore()));
    }

    SECTION ("assign a1, a2; Select <a1.stmt#, a2> such that Affects (a1, a2)") {
        std::string queryStr = "assign a1, a2; Select <a1.stmt#, a2> such that Affects (a1, a2)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a1")),
                        Declaration(DesignEntity::ASSIGN, Synonym("a2"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(AttrRef(Synonym("a1"), AttrName::STMT_NUMBER));
        returnResults->push_back(Synonym("a2"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<AffectsClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == AffectsClause(Synonym("a1"), Synonym("a2")));
    }

    SECTION ("Select BOOLEAN such that Next* (2, 9)") {
        std::string queryStr = "Select BOOLEAN such that Next* (2, 9)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::BOOLEAN));
        REQUIRE(query->suchThatClauses->size() == 1);
        auto clause = dynamic_pointer_cast<NextTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == NextTClause(2, 9));
    }

    SECTION ("Select BOOLEAN such that Next* (2, 9) and Next (9, 10)") {
        std::string queryStr = "Select BOOLEAN such that Next* (2, 9) and Next (9, 10)";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::BOOLEAN));
        REQUIRE(query->suchThatClauses->size() == 2);
        auto clause = dynamic_pointer_cast<NextTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == NextTClause(2, 9));
        auto clause2 = dynamic_pointer_cast<NextClause>((query->suchThatClauses)->at(1));
        REQUIRE(*clause2 == NextClause(9, 10));
    }

    SECTION ("procedure p; variable v; Select p with p.procName = v.varName") {
        std::string queryStr = "procedure p; variable v; Select p with p.procName = v.varName";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 1);
        AttrRef lhs = AttrRef(Synonym("p"), AttrName::PROC_NAME);
        AttrRef rhs = AttrRef(Synonym("v"), AttrName::VAR_NAME);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(lhs, rhs));
    }

    SECTION ("procedure p; variable v; Select p with p.procName = \"test\"") {
        std::string queryStr = "procedure p; variable v; Select p with p.procName = \"test\"";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 1);
        AttrRef lhs = AttrRef(Synonym("p"), AttrName::PROC_NAME);
        Ident rhs = Ident("test");
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(lhs, rhs));
    }

    SECTION ("procedure p; variable v; Select p with 1 = 1") {
        std::string queryStr = "procedure p; variable v; Select p with 1 = 1";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 1);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(1, 1));
    }

    SECTION ("procedure p; variable v; stmt s; constant c; Select p with p.procName = v.varName and s.stmt# = c.value") {
        std::string queryStr = "procedure p; variable v; stmt s; constant c; Select p with p.procName = v.varName and s.stmt# = c.value";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 4);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::PROCEDURE, Synonym("p")),
                        Declaration(DesignEntity::VARIABLE, Synonym("v")),
                        Declaration(DesignEntity::STMT, Synonym("s")),
                        Declaration(DesignEntity::CONSTANT, Synonym("c"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("p"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 2);
        AttrRef lhs = AttrRef(Synonym("p"), AttrName::PROC_NAME);
        AttrRef rhs = AttrRef(Synonym("v"), AttrName::VAR_NAME);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(lhs, rhs));
        AttrRef lhs2 = AttrRef(Synonym("s"), AttrName::STMT_NUMBER);
        AttrRef rhs2 = AttrRef(Synonym("c"), AttrName::VALUE);
        REQUIRE(*(query->withClauses)->at(1) ==
                WithClause(lhs2, rhs2));
    }

    SECTION ("stmt s; constant c; Select s with s.stmt# = c.value") {
        std::string queryStr = "stmt s; constant c; Select s with s.stmt# = c.value";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("s")),
                        Declaration(DesignEntity::CONSTANT, Synonym("c"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("s"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 1);
        AttrRef lhs = AttrRef(Synonym("s"), AttrName::STMT_NUMBER);
        AttrRef rhs = AttrRef(Synonym("c"), AttrName::VALUE);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(lhs, rhs));
    }

    SECTION ("stmt BOOLEAN; constant c; Select <BOOLEAN> with BOOLEAN.stmt# = c.value") {
        std::string queryStr = "stmt BOOLEAN; constant c; Select BOOLEAN with BOOLEAN.stmt# = c.value";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 2);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::STMT, Synonym("BOOLEAN")),
                        Declaration(DesignEntity::CONSTANT, Synonym("c"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("BOOLEAN"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 1);
        AttrRef lhs = AttrRef(Synonym("BOOLEAN"), AttrName::STMT_NUMBER);
        AttrRef rhs = AttrRef(Synonym("c"), AttrName::VALUE);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(lhs, rhs));
    }

    SECTION ("assign a; while w; stmt s; Select <a, w> such that Parent* (w, a) and Next* (60, s) pattern w(\"x\", _)"
             "with a.stmt# = s.stmt#") {
        std::string queryStr = "assign a; while w; stmt s; Select <a, w> such that Parent* (w, a) and Next* (60, s) "
                            "pattern w(\"x\", _) with a.stmt# = s.stmt#";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 3);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::ASSIGN, Synonym("a")),
                        Declaration(DesignEntity::WHILE, Synonym("w")),
                        Declaration(DesignEntity::STMT, Synonym("s"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("a"));
        returnResults->push_back(Synonym("w"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->suchThatClauses->size() == 2);
        auto clause = dynamic_pointer_cast<ParentTClause>((query->suchThatClauses)->at(0));
        REQUIRE(*clause == ParentTClause(Synonym("w"), Synonym("a")));
        auto clause2 = dynamic_pointer_cast<NextTClause>((query->suchThatClauses)->at(1));
        REQUIRE(*clause2 == NextTClause(60, Synonym("s")));
        REQUIRE(query->patternClauses->size() == 1);
        WhilePatternClause whilePatternClause = dynamic_cast<WhilePatternClause&>( *(query->patternClauses->at(0)));
        REQUIRE(whilePatternClause == WhilePatternClause(
                Synonym("w"),
                Ident("x")));
        REQUIRE(query->withClauses->size() == 1);
        AttrRef lhs = AttrRef(Synonym("a"), AttrName::STMT_NUMBER);
        AttrRef rhs = AttrRef(Synonym("s"), AttrName::STMT_NUMBER);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(lhs, rhs));
    }

    SECTION ("constant c; Select c with c.value = 1") {
        std::string queryStr = "constant c; Select c with c.value = 1";
        auto query = queryBuilder->buildPQLQuery(queryStr);
        REQUIRE(query->declarations->size() == 1);
        REQUIRE(*(query->declarations) ==
                std::vector<Declaration>{
                        Declaration(DesignEntity::CONSTANT, Synonym("c"))});
        shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
        returnResults->push_back(Synonym("c"));
        REQUIRE(*(query->selectClause) ==
                SelectClause(ReturnType::TUPLE, returnResults));
        REQUIRE(query->withClauses->size() == 1);
        REQUIRE(*(query->withClauses)->at(0) ==
                WithClause(AttrRef(Synonym("c"), AttrName::VALUE), 1));
    }

    SECTION ("'select' is not defined, throw PQLParseException") {
        std::string queryStr = "variable v; select v";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Unexpected token ';', throw PQLParseException") {
        std::string queryStr = "variable v; Select v;";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("'uses' is not defined, throw PQLParseException") {
        std::string queryStr = "variable v; Select v such that uses (1, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("'such that' is not found, throw PQLParseException") {
        std::string queryStr = "variable v; Select v Uses (1, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("no ';' after declaration, throw PQLParseException") {
        std::string queryStr = "variable v Select v such that Uses (1, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("no ';' after declaration, found ',', throw PQLParseException") {
        std::string queryStr = "variable v, Select v such that Uses (1, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid synonym in Ref arg, throw PQLParseException") {
        std::string queryStr = "variable 123abc;";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid synonym declared, throw PQLParseException") {
        std::string queryStr = "variable v; Select v such that Uses (2, 123abc)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid number in SuchThat Clause, throw PQLParseException") {
        std::string queryStr = "variable v; Select v such that Uses (0123, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid Ref in SuchThat Clause, throw PQLParseException") {
        std::string queryStr = "variable v; Select v such that Uses (=, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid number, throw PQLParseException") {
        std::string queryStr = "stmt BOOLEAN; Select BOOLEAN with 0123 = 0123";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid Ref, throw PQLParseException") {
        std::string queryStr = "variable v, Select v such that Uses ((,), v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Empty arg 2 in pattern clause, throw PQLParseException") {
        std::string queryStr = "assign a, Select a pattern a (, _)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("'Pattern' is not defined, throw PQLParseException") {
        std::string queryStr = "assign a, Select a Pattern a (_, _)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid expression (number), throw PQLParseException") {
        std::string queryStr = "assign a, Select a pattern a (_, 123)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid expression (synonym), throw PQLParseException") {
        std::string queryStr = "assign a, Select a pattern a (_, x)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid tuple syntax, throw PQLParseException") {
        std::string queryStr = "variable a; Select <a pattern a (_, _)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid 'and' connector, throw PQLParseException") {
        std::string queryStr = "if i; Select BOOLEAN such that Follows (9, 10) and pattern i (2, _, _)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid expressionSpec, throw PQLParseException") {
        std::string queryStr = "assign a; Select a pattern a (_, _\" +temp\"_)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid expressionSpec, throw PQLParseException") {
        std::string queryStr = "assign a; Select a pattern a (_, _\" temp+ \"_)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Empty expressionSpec, throw PQLParseException") {
        std::string queryStr = "assign a; Select a pattern a (_, \"\")";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Empty assign expression, throw PQLParseException") {
        std::string queryStr = "assign a; Select a pattern a (_, _\"\"_)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid pattern arg2, throw PQLParseException") {
        std::string queryStr = "assign a; Select a pattern a (_, 1)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid pattern clause, throw PQLParseException") {
        std::string queryStr = "assign a; stmt s; variable v; Select s pattern Modifies(s, v)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }

    SECTION ("Invalid ident in while clause, throw PQLParseException") {
        std::string queryStr = "while w; Select w pattern w(\"10\", _)";
        REQUIRE_THROWS_AS(queryBuilder->buildPQLQuery(queryStr), PQLParseException);
    }
}
