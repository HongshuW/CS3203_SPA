//
// Created by Xingchen Lin on 28/8/22.
//

#include <memory>
#include <string>
#include <vector>

#include "catch.hpp"
#include "query_builder/QueryBuilder.h"
#include "query_builder/QueryParser.h"
#include "query_builder/QueryTokenizer.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "query_builder/commons/Ref.h"
#include "query_builder/exceptions/Exceptions.h"
#include "query_builder/relations/such_that_relations/Affects.h"
#include "query_builder/relations/such_that_relations/AffectsT.h"
#include "query_builder/relations/such_that_relations/Calls.h"
#include "query_builder/relations/such_that_relations/CallsT.h"
#include "query_builder/relations/such_that_relations/Follows.h"
#include "query_builder/relations/such_that_relations/FollowsT.h"
#include "query_builder/relations/such_that_relations/ModifiesP.h"
#include "query_builder/relations/such_that_relations/ModifiesS.h"
#include "query_builder/relations/such_that_relations/Next.h"
#include "query_builder/relations/such_that_relations/NextT.h"
#include "query_builder/relations/such_that_relations/Parent.h"
#include "query_builder/relations/such_that_relations/ParentT.h"
#include "query_builder/relations/such_that_relations/UsesP.h"
#include "query_builder/relations/such_that_relations/UsesS.h"

using namespace QB;

TEST_CASE("Test Query Parser") {
  SECTION("variable v1; Select v1") {
    auto tokens = vector<string>({"variable", "v1", ";", "Select", "v1"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::VARIABLE, Synonym("v1"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("v1"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
  }

  SECTION("stmt a, b, c; Select a") {
    auto tokens =
        vector<string>({"stmt", "a", ",", "b", ",", "c", ";", "Select", "a"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 3);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::STMT, Synonym("a")),
                Declaration(DesignEntity::STMT, Synonym("b")),
                Declaration(DesignEntity::STMT, Synonym("c"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
  }

  SECTION("assign a; while w; Select a") {
    auto tokens =
        vector<string>({"assign", "a", ";", "while", "w", ";", "Select", "a"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 2);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::ASSIGN, Synonym("a")),
                Declaration(DesignEntity::WHILE, Synonym("w"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
  }

  SECTION("stmt s; Select s such that Parent (s, 12)") {
    auto tokens = vector<string>({"stmt", "s", ";", "Select", "s", "such",
                                  "that", "Parent", "(", "s", ",", "12", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::STMT, Synonym("s"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("s"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<ParentClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ParentClause(Synonym("s"), 12));
  }

  SECTION("assign a; Select a such that Parent* (_, a)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "such",
                                  "that", "Parent*", "(", "_", ",", "a", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<ParentTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ParentTClause(Underscore(), Synonym("a")));
  }

  SECTION("stmt s; Select s such that Follows (6, s)") {
    auto tokens = vector<string>({"stmt", "s", ";", "Select", "s", "such",
                                  "that", "Follows", "(", "6", ",", "s", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::STMT, Synonym("s"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("s"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<FollowsClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == FollowsClause(6, Synonym("s")));
  }

  SECTION("stmt s; Select s such that Follows* (s, 6)") {
    auto tokens = vector<string>({"stmt", "s", ";", "Select", "s", "such",
                                  "that", "Follows*", "(", "s", ",", "6", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::STMT, Synonym("s"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("s"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<FollowsTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == FollowsTClause(Synonym("s"), 6));
  }

  SECTION("stmt s; Select s such that Follows* (s, _)") {
    auto tokens = vector<string>({"stmt", "s", ";", "Select", "s", "such",
                                  "that", "Follows*", "(", "s", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::STMT, Synonym("s"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("s"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<FollowsTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == FollowsTClause(Synonym("s"), Underscore()));
  }

  SECTION("assign a; Select a such that Uses (a, \"x\")") {
    auto tokens =
        vector<string>({"assign", "a", ";", "Select", "a", "such", "that",
                        "Uses", "(", "a", ",", "\"", "x", "\"", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == UsesSClause(Synonym("a"), Ident("x")));
  }

  SECTION("variable v; Select v such that Modifies (\"main\", v)") {
    auto tokens =
        vector<string>({"variable", "v", ";", "Select", "v", "such", "that",
                        "Modifies", "(", "\"", "main", "\"", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::VARIABLE, Synonym("v"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("v"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<ModifiesPClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ModifiesPClause(Ident("main"), Synonym("v")));
  }

  SECTION("variable v; Select v such that Modifies (8, v)") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", "such",
                                  "that", "Modifies", "(", "8", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::VARIABLE, Synonym("v"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("v"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<ModifiesSClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ModifiesSClause(8, Synonym("v")));
  }

  SECTION("stmt s; Select s such that Modifies (s, _)") {
    auto tokens = vector<string>({"stmt", "s", ";", "Select", "s", "such",
                                  "that", "Modifies", "(", "s", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::STMT, Synonym("s"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("s"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<ModifiesSClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ModifiesSClause(Synonym("s"), Underscore()));
  }

  SECTION("procedure p; Select p such that Modifies (p, _)") {
    auto tokens = vector<string>({"procedure", "p", ";", "Select", "p", "such",
                                  "that", "Modifies", "(", "p", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("p"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<ModifiesPClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ModifiesPClause(Synonym("p"), Underscore()));
  }

  SECTION("assign a; Select a such that Uses (a, _)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "such",
                                  "that", "Uses", "(", "a", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == UsesSClause(Synonym("a"), Underscore()));
  }

  SECTION("procedure p; Select p such that Uses (p, _)") {
    auto tokens = vector<string>({"procedure", "p", ";", "Select", "p", "such",
                                  "that", "Uses", "(", "p", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("p"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<UsesPClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == UsesPClause(Synonym("p"), Underscore()));
  }

  SECTION("procedure p; Select p such that Calls* (p, _)") {
    auto tokens = vector<string>({"procedure", "p", ";", "Select", "p", "such",
                                  "that", "Calls*", "(", "p", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::PROCEDURE, Synonym("p"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("p"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<CallsTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == CallsTClause(Synonym("p"), Underscore()));
  }

  SECTION("while w; Select w such that Next (w, 2)") {
    auto tokens = vector<string>({"while", "w", ";", "Select", "w", "such",
                                  "that", "Next", "(", "w", ",", "2", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::WHILE, Synonym("w"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("w"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<NextClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == NextClause(Synonym("w"), 2));
  }

  SECTION("assign w; Select w such that Affects* (_, w)") {
    auto tokens = vector<string>({"assign", "w", ";", "Select", "w", "such",
                                  "that", "Affects*", "(", "_", ",", "w", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("w"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("w"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<AffectsTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == AffectsTClause(Underscore(), Synonym("w")));
  }

  SECTION("assign a; Select a pattern a (\"test\", _)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", "\"", "test", "\"", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(assignPatternClause ==
            AssignPatternClause(Synonym("a"), Ident("test"),
                                ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
  }

  SECTION("assign a; Select a pattern a (_, _)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", "_", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(assignPatternClause ==
            AssignPatternClause(Synonym("a"), Underscore(),
                                ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
  }

  SECTION("assign a; Select a pattern a (_, _) and a (_, \"x + 1\")") {
    auto tokens =
        vector<string>({"assign", "a", ";", "Select", "a",     "pattern", "a",
                        "(",      "_", ",", "_",      ")",     "and",     "a",
                        "(",      "_", ",", "\"",     "x + 1", "\"",      ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(assignPatternClause ==
            AssignPatternClause(Synonym("a"), Underscore(),
                                ExpressionSpec(ExpressionSpecType::ANY_MATCH)));
    AssignPatternClause assignPatternClause2 =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(1)));
    shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("+");
    exprNode->left = make_shared<ExprNode>("x");
    exprNode->right = make_shared<ExprNode>("1");
    REQUIRE(assignPatternClause2 ==
            AssignPatternClause(
                Synonym("a"), Underscore(),
                ExpressionSpec(ExpressionSpecType::FULL_MATCH, exprNode)));
  }

  SECTION("assign a; variable x; Select a pattern a (_, \"x + 1\")") {
    auto tokens = vector<string>({"assign", "a", ";", "variable", "x", ";",
                                  "Select", "a", "pattern", "a", "(", "_", ",",
                                  "\"", "x + 1", "\"", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(assignPatternClause ==
            AssignPatternClause(
                Synonym("a"), Underscore(),
                ExpressionSpec(ExpressionSpecType::FULL_MATCH, exprNode)));
  }

  SECTION("assign a; Select a pattern a (_, \"(2 + 1)\")") {
    auto tokens =
        vector<string>({"assign", "a", ";", "Select", "a", "pattern", "a", "(",
                        "_", ",", "\"", "(2 + 1)", "\"", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) == std::vector<Declaration>{Declaration(
                                          DesignEntity::ASSIGN, Synonym("a"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("a"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    shared_ptr<ExprNode> exprNode = make_shared<ExprNode>("+");
    exprNode->left = make_shared<ExprNode>("2");
    exprNode->right = make_shared<ExprNode>("1");
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(assignPatternClause ==
            AssignPatternClause(
                Synonym("a"), Underscore(),
                ExpressionSpec(ExpressionSpecType::FULL_MATCH, exprNode)));
  }

  SECTION("assign a; variable x; Select a pattern a (_, _\"(x + 1) * 3\"_)") {
    auto tokens = vector<string>({"assign", "a", ";", "variable", "x", ";",
                                  "Select", "a", "pattern", "a", "(", "_", ",",
                                  "_", "\"", "(x + 1) * 3", "\"", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(assignPatternClause ==
            AssignPatternClause(
                Synonym("a"), Underscore(),
                ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH, exprNode)));
  }

  SECTION(
      "assign a; variable v, y; Select a such that Uses (a, \"x\") pattern a "
      "(v, _\"y\"_)") {
    auto tokens = vector<string>(
        {"assign", "a", ";",    "variable", "v",       ",", "y", ";",
         "Select", "a", "such", "that",     "Uses",    "(", "a", ",",
         "\"",     "x", "\"",   ")",        "pattern", "a", "(", "v",
         ",",      "_", "\"",   "y",        "\"",      "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    auto clause =
        dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == UsesSClause(Synonym("a"), Ident("x")));
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(
        assignPatternClause ==
        AssignPatternClause(Synonym("a"), Synonym("v"),
                            ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
                                           make_shared<ExprNode>("y"))));
  }

  SECTION(
      "assign a; variable v; Select v pattern a (v, _\"y\"_) such that Uses "
      "(a, \"x\")") {
    auto tokens = vector<string>(
        {"assign",  "a",  ";", "variable", "v",    ";",    "Select", "v",
         "pattern", "a",  "(", "v",        ",",    "_",    "\"",     "y",
         "\"",      "_",  ")", "such",     "that", "Uses", "(",      "a",
         ",",       "\"", "x", "\"",       ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    auto clause =
        dynamic_pointer_cast<UsesSClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == UsesSClause(Synonym("a"), Ident("x")));
    AssignPatternClause assignPatternClause =
        dynamic_cast<AssignPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(
        assignPatternClause ==
        AssignPatternClause(Synonym("a"), Synonym("v"),
                            ExpressionSpec(ExpressionSpecType::PARTIAL_MATCH,
                                           make_shared<ExprNode>("y"))));
  }

  SECTION("if i; variable v; Select v pattern i (v, _, _)") {
    auto tokens =
        vector<string>({"if", "i", ";", "variable", "v", ";", "Select", "v",
                        "pattern", "i", "(", "v", ",", "_", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 2);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::IF, Synonym("i")),
                Declaration(DesignEntity::VARIABLE, Synonym("v"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("v"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    IfPatternClause ifPatternClause =
        dynamic_cast<IfPatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(ifPatternClause == IfPatternClause(Synonym("i"), Synonym("v")));
  }

  SECTION("variable v; Select v such that Calls (_, \"testProcedure\")") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", "such",
                                  "that", "Calls", "(", "_", ",", "\"",
                                  "testProcedure", "\"", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::VARIABLE, Synonym("v"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("v"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    auto clause =
        dynamic_pointer_cast<CallsClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == CallsClause(Underscore(), Ident("testProcedure")));
  }

  SECTION("variable v; Select v such that Calls* (_, _)") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", "such",
                                  "that", "Calls*", "(", "_", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(query->declarations->size() == 1);
    REQUIRE(*(query->declarations) ==
            std::vector<Declaration>{
                Declaration(DesignEntity::VARIABLE, Synonym("v"))});
    shared_ptr<vector<Elem>> returnResults = make_shared<vector<Elem>>();
    returnResults->push_back(Synonym("v"));
    REQUIRE(*(query->selectClause) ==
            SelectClause(ReturnType::TUPLE, returnResults));
    auto clause =
        dynamic_pointer_cast<CallsTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == CallsTClause(Underscore(), Underscore()));
  }

  SECTION("assign a1, a2; Select <a1.stmt#, a2> such that Affects (a1, a2)") {
    auto tokens = vector<string>(
        {"assign", "a1",      ",",     "a2", ";",  "Select", "<",
         "a1",     ".",       "stmt#", ",",  "a2", ">",      "such",
         "that",   "Affects", "(",     "a1", ",",  "a2",     ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    auto clause =
        dynamic_pointer_cast<AffectsClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == AffectsClause(Synonym("a1"), Synonym("a2")));
  }

  SECTION("Select BOOLEAN such that Next* (2, 9)") {
    auto tokens = vector<string>({"Select", "BOOLEAN", "such", "that", "Next*",
                                  "(", "2", ",", "9", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(*(query->selectClause) == SelectClause(ReturnType::BOOLEAN));
    REQUIRE(query->suchThatClauses->size() == 1);
    auto clause =
        dynamic_pointer_cast<NextTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == NextTClause(2, 9));
  }

  SECTION("Select BOOLEAN such that Next* (2, 9) and Next (9, 10)") {
    auto tokens = vector<string>({"Select", "BOOLEAN", "such", "that", "Next*",
                                  "(", "2", ",", "9", ")", "and", "Next", "(",
                                  "9", ",", "10", ")"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
    REQUIRE(*(query->selectClause) == SelectClause(ReturnType::BOOLEAN));
    REQUIRE(query->suchThatClauses->size() == 2);
    auto clause =
        dynamic_pointer_cast<NextTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == NextTClause(2, 9));
    auto clause2 =
        dynamic_pointer_cast<NextClause>((query->suchThatClauses)->at(1));
    REQUIRE(*clause2 == NextClause(9, 10));
  }

  SECTION("procedure p; variable v; Select p with p.procName = v.varName") {
    auto tokens = vector<string>({"procedure", "p", ";", "variable", "v", ";",
                                  "Select", "p", "with", "p", ".", "procName",
                                  "=", "v", ".", "varName"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    REQUIRE(*(query->withClauses)->at(0) == WithClause(lhs, rhs));
  }

  SECTION("procedure p; variable v; Select p with p.procName = \"test\"") {
    auto tokens = vector<string>({"procedure", "p", ";", "variable", "v", ";",
                                  "Select", "p", "with", "p", ".", "procName",
                                  "=", "\"", "test", "\""});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    REQUIRE(*(query->withClauses)->at(0) == WithClause(lhs, rhs));
  }

  SECTION("procedure p; variable v; Select p with 1 = 1") {
    auto tokens = vector<string>({"procedure", "p", ";", "variable", "v", ";",
                                  "Select", "p", "with", "1", "=", "1"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    REQUIRE(*(query->withClauses)->at(0) == WithClause(1, 1));
  }

  SECTION(
      "procedure p; variable v; stmt s; constant c; Select p with p.procName = "
      "v.varName and s.stmt# = c.value") {
    auto tokens = vector<string>(
        {"procedure", "p",     ";",    "variable", "v",   ";",
         "stmt",      "s",     ";",    "constant", "c",   ";",
         "Select",    "p",     "with", "p",        ".",   "procName",
         "=",         "v",     ".",    "varName",  "and", "s",
         ".",         "stmt#", "=",    "c",        ".",   "value"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    REQUIRE(*(query->withClauses)->at(0) == WithClause(lhs, rhs));
    AttrRef lhs2 = AttrRef(Synonym("s"), AttrName::STMT_NUMBER);
    AttrRef rhs2 = AttrRef(Synonym("c"), AttrName::VALUE);
    REQUIRE(*(query->withClauses)->at(1) == WithClause(lhs2, rhs2));
  }

  SECTION("stmt s; constant c; Select s with s.stmt# = c.value") {
    auto tokens =
        vector<string>({"stmt", "s", ";", "constant", "c", ";", "Select", "s",
                        "with", "s", ".", "stmt#", "=", "c", ".", "value"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    REQUIRE(*(query->withClauses)->at(0) == WithClause(lhs, rhs));
  }

  SECTION(
      "stmt BOOLEAN; constant c; Select <BOOLEAN> with BOOLEAN.stmt# = "
      "c.value") {
    auto tokens = vector<string>(
        {"stmt", "BOOLEAN", ";", "constant", "c", ";", "Select", "<", "BOOLEAN",
         ">", "with", "BOOLEAN", ".", "stmt#", "=", "c", ".", "value"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    REQUIRE(*(query->withClauses)->at(0) == WithClause(lhs, rhs));
  }

  SECTION(
      "assign a; while w; stmt s; Select <a, w> such that Parent* (w, a) and "
      "Next* (60, s) pattern w(\"x\", _)"
      "with a.stmt# = s.stmt#") {
    auto tokens = vector<string>(
        {"assign", "a",       ";",     "while", "w", ";", "stmt",    "s",
         ";",      "Select",  "<",     "a",     ",", "w", ">",       "such",
         "that",   "Parent*", "(",     "w",     ",", "a", ")",       "and",
         "Next*",  "(",       "60",    ",",     "s", ")", "pattern", "w",
         "(",      "\"",      "x",     "\"",    ",", "_", ")",       "with",
         "a",      ".",       "stmt#", "=",     "s", ".", "stmt#"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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
    auto clause =
        dynamic_pointer_cast<ParentTClause>((query->suchThatClauses)->at(0));
    REQUIRE(*clause == ParentTClause(Synonym("w"), Synonym("a")));
    auto clause2 =
        dynamic_pointer_cast<NextTClause>((query->suchThatClauses)->at(1));
    REQUIRE(*clause2 == NextTClause(60, Synonym("s")));
    REQUIRE(query->patternClauses->size() == 1);
    WhilePatternClause whilePatternClause =
        dynamic_cast<WhilePatternClause&>(*(query->patternClauses->at(0)));
    REQUIRE(whilePatternClause == WhilePatternClause(Synonym("w"), Ident("x")));
    REQUIRE(query->withClauses->size() == 1);
    AttrRef lhs = AttrRef(Synonym("a"), AttrName::STMT_NUMBER);
    AttrRef rhs = AttrRef(Synonym("s"), AttrName::STMT_NUMBER);
    REQUIRE(*(query->withClauses)->at(0) == WithClause(lhs, rhs));
  }

  SECTION("constant c; Select c with c.value = 1") {
    auto tokens = vector<string>({"constant", "c", ";", "Select", "c", "with",
                                  "c", ".", "value", "=", "1"});
    auto parser = QueryParser(tokens);
    auto query = parser.parse();
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

  SECTION("'select' is not defined - variable v; select v") {
    auto tokens = vector<string>({"variable", "v", ";", "select", "v"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Unexpected token ';' - variable v; Select v;") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", ";"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "'uses' is not defined - variable v; Select v such that uses (1, v)") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", "such",
                                  "that", "uses", "(", "1", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("'such that' is not found - variable v; Select v Uses (1, v)") {
    auto tokens = vector<string>(
        {"variable", "v", ";", "Select", "v", "Uses", "(", "1", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "No ';' after declaration - variable v Select v such that Uses (1, v)") {
    auto tokens = vector<string>({"variable", "v", "Select", "v", "such",
                                  "that", "Uses", "(", "1", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "No ';' after declaration, found ',' - variable v, Select v such that "
      "Uses (1, v)") {
    auto tokens = vector<string>({"variable", "v", ",", "Select", "v", "such",
                                  "that", "Uses", "(", "1", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Invalid synonym in Ref arg - variable 123abc;") {
    auto tokens = vector<string>({"variable", "123abc", ";"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Synonym not declared - variable v; Select v such that Uses (2, "
      "123abc)") {
    auto tokens =
        vector<string>({"variable", "v", ";", "Select", "v", "such", "that",
                        "Uses", "(", "2", ",", "123abc", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid number in SuchThat Clause - variable v; Select v such that Uses "
      "(0123, v)") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", "such",
                                  "that", "Uses", "(", "0123", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid Ref in SuchThat Clause - variable v; Select v such that Uses "
      "(=, v)") {
    auto tokens = vector<string>({"variable", "v", ";", "Select", "v", "such",
                                  "that", "Uses", "(", "=", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Invalid number - stmt BOOLEAN; Select BOOLEAN with 0123 = 0123") {
    auto tokens = vector<string>({"stmt", "BOOLEAN", ";", "Select", "BOOLEAN",
                                  "with", "0123", "=", "0123"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Invalid Ref - variable v; Select v such that Uses ((,), v)") {
    auto tokens =
        vector<string>({"variable", "v", ";", "Select", "v", "such", "that",
                        "Uses", "(", "(", ",", ")", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Empty arg 2 in pattern clause - assign a; Select a pattern a (, _)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("'Pattern' is not defined - assign a; Select a Pattern a (_, _)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "Pattern",
                                  "a", "(", "_", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid expression (number) - assign a; Select a pattern a (_, 123)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", "_", ",", "123", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid expression (synonym) - assign a; Select a pattern a (_, x)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", "_", ",", "x", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Invalid tuple syntax - variable a; Select <a pattern a (_, _)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "<", "a",
                                  "pattern", "a", "(", "_", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid 'and' connector - if i; Select BOOLEAN such that Follows (9, "
      "10) and pattern i (2, _, _)") {
    auto tokens = vector<string>(
        {"if", "i", ";", "Select", "BOOLEAN", "such", "that",    "Follows",
         "(",  "9", ",", "10",     ")",       "and",  "pattern", "i",
         "(",  "2", "_", ",",      "_",       ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid expressionSpec - assign a; Select a pattern a (_, _\" "
      "+temp\"_)") {
    auto tokens =
        vector<string>({"assign", "a", ";", "Select", "a", "pattern", "a", "(",
                        "_", ",", "_", "\"", " +temp", "\"", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid expressionSpec - assign a; Select a pattern a (_, _\" temp+ "
      "\"_)") {
    auto tokens =
        vector<string>({"assign", "a", ";", "Select", "a", "pattern", "a", "(",
                        "_", ",", "_", "\"", " temp+", "\"", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Empty expressionSpec - assign a; Select a pattern a (_, \"\")") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", "_", ",", "\"", "\"", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Empty assign expression - assign a; Select a pattern a (_, _\"\"_)") {
    auto tokens =
        vector<string>({"assign", "a", ";", "Select", "a", "pattern", "a", "(",
                        "_", ",", "_", "\"", "\"", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Invalid pattern arg2 - assign a; Select a pattern a (_, 1)") {
    auto tokens = vector<string>({"assign", "a", ";", "Select", "a", "pattern",
                                  "a", "(", "_", ",", "1", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid pattern clause - assign a; stmt s; variable v; Select s pattern "
      "Modifies(s, v)") {
    auto tokens =
        vector<string>({"assign", "a", ";", "stmt", "s", ";", "Select", "s",
                        "pattern", "Modifies", "(", "s", ",", "v", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid ident in while clause - while w; Select w pattern w(\"10\", "
      "_)") {
    auto tokens = vector<string>({"while", "w", ";", "Select", "w", "pattern",
                                  "w", "(", "\"", "10", "\"", ",", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION("Negative number - stmt BOOLEAN; Select BOOLEAN with 123 = -123") {
    auto tokens = vector<string>({"stmt", "BOOLEAN", ";", "Select", "BOOLEAN",
                                  "with", "123", "=", "-123"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Syntactically Valid query - stmt s; read r; print p; variable v; Select "
      "s such that Follows(r, p) pattern a(v, _\"100\"_)") {
    auto tokens = vector<string>(
        {"stmt", "s",       ";",        "read", "r", ";",      "print",
         "p",    ";",       "variable", "v",    ";", "Select", "s",
         "such", "that",    "Follows",  "(",    "r", ",",      "p",
         ")",    "pattern", "a",        "(",    "v", ",",      "_",
         "\"",   "100",     "\"",       "_",    ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION(
      "Syntactically Valid query - variable v; assign a; Select v such that "
      "Uses(a,v) pattern a(_,_\"2\"_)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",       "assign", "a", ";", "Select",
         "v",        "such", "that",    "Uses",   "(", "a", ",",
         "v",        ")",    "pattern", "a",      "(", "_", ",",
         "_",        "\"",   "2",       "\"",     "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION(
      "Syntactically Valid query - variable v; Select v such that Uses(a,v) "
      "pattern a(_,\"2\")") {
    auto tokens = vector<string>(
        {"variable", "v", ";",  "Select", "v",  "such",    "that", "Uses",
         "(",        "a", ",",  "v",      ")",  "pattern", "a",    "(",
         "_",        ",", "\"", "2",      "\"", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION(
      "Syntactically Valid query - variable v; Select v such that Uses(a,v) "
      "pattern a(_, _)") {
    auto tokens =
        vector<string>({"variable", "v", ";", "Select", "v", "such", "that",
                        "Uses",     "(", "a", ",",      "v", ")",    "pattern",
                        "a",        "(", "_", ",",      "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION(
      "Syntactically Valid query - variable v; procedure p; Select v such that "
      "Uses(a,v) pattern v(p, _)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",    "procedure", "p", ";", "Select", "v",
         "such",     "that", "Uses", "(",         "a", ",", "v",      ")",
         "pattern",  "v",    "(",    "p",         ",", "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION(
      "Syntactically Valid query - variable v; procedure p; Select v such that "
      "Uses(a,v) pattern v(p, _, _)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",       "procedure", "p", ";", "Select",
         "v",        "such", "that",    "Uses",      "(", "a", ",",
         "v",        ")",    "pattern", "v",         "(", "p", ",",
         "_",        ",",    "_",       ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_NOTHROW(parser.parse());
  }

  SECTION(
      "Invalid expression spec - variable v; assign a; Select v such that "
      "Uses(a,v) pattern a(_, _\"+ 2\"_)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",       "assign", "a", ";", "Select",
         "v",        "such", "that",    "Uses",   "(", "a", ",",
         "v",        ")",    "pattern", "a",      "(", "_", ",",
         "_",        "\"",   "+ 2",     "\"",     "_", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid while pattern rules - variable v; procedure p; Select v such "
      "that Uses(a,v) pattern v(p, 1)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",    "procedure", "p", ";", "Select", "v",
         "such",     "that", "Uses", "(",         "a", ",", "v",      ")",
         "pattern",  "v",    "(",    "p",         ",", "1", ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid if pattern rules - variable v; procedure p; Select v such that "
      "Uses(a,v) pattern v(p, _, x)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",       "procedure", "p", ";", "Select",
         "v",        "such", "that",    "Uses",      "(", "a", ",",
         "v",        ")",    "pattern", "v",         "(", "p", ",",
         "_",        ",",    "x",       ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }

  SECTION(
      "Invalid assign pattern rules - variable v; procedure p; Select v such "
      "that Uses(a,v) pattern v(p, _\"\"_)") {
    auto tokens = vector<string>(
        {"variable", "v",    ";",       "procedure", "p", ";", "Select",
         "v",        "such", "that",    "Uses",      "(", "a", ",",
         "v",        ")",    "pattern", "v",         "(", "p", ",",
         "_",        "\"",   "\"",      "_",         ")"});
    auto parser = QueryParser(tokens);
    REQUIRE_THROWS_AS(parser.parse(), PQLParseException);
  }
}
