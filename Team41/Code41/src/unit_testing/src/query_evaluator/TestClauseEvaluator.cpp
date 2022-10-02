////
//// Created by Nafour on 28/9/22.
////
//#include "catch.hpp"
//#include "DummyDataRetrievers/DummyDataRetriever.h"
//#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
//#include "query_evaluator/DataPreprocessor.h"
//#include "query_builder/commons/Declaration.h"
//#include <iostream>
//#include <variant>
//
//using namespace QE;;
//TEST_CASE("Test Clause evaluator") {
//    SECTION("Sanity check") {
//        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
//        shared_ptr<DummyDataRetriever> dummyDataRetriever = make_shared<DummyDataRetriever>(make_shared<PKBStorage>());
//        shared_ptr<DataPreprocessor> dataPreprocessor = make_shared<DataPreprocessor>(dummyDataRetriever, declarations);
//
//        shared_ptr<ClauseVisitor> clauseEvaluator = make_shared<ClauseVisitor>(dataPreprocessor, declarations);
//
//        Synonym syn1 = Synonym("a1");
//        Synonym syn2 = Synonym("a2");
//        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
//        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
//        Clause suchThatClause = make_shared<SuchThatClause>(QB::RelationType::FOLLOWS, syn1, syn2, declarations);
//
//        std::visit(*clauseEvaluator, suchThatClause);
//        ExpressionSpec expressionSpec = ExpressionSpec(QB::ExpressionSpecType::ANY_MATCH);
//        shared_ptr<PatternClause> patternClause = make_shared<AssignPatternClause>( syn1, Ident("dummyVarA"), expressionSpec);
//        Clause clause = patternClause->asClauseVariant();
//        std::visit(*clauseEvaluator, clause);
//
//    }
//}