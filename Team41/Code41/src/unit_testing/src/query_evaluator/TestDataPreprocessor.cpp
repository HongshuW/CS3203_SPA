//
// Created by Nafour on 5/9/22.
//
#include "catch.hpp"
#include "query_builder/clauses/SuchThatClause.h"
#include "query_evaluator/DataPreprocessor.h"

TEST_CASE("Test Data Preprocessor") {
    //todo: replace fakeDR withh dummyDR
    shared_ptr<DataRetriever> dataRetriever = make_shared<DataRetriever>();
    shared_ptr<QE::DataPreprocessor> dataPreprocessor = make_shared<QE::DataPreprocessor>(QE::DataPreprocessor(dataRetriever));
    SECTION("section 1") {
        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
        Synonym syn1 = Synonym("a1");
        Synonym syn2 = Synonym("a2");
        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
        declarations->push_back({Declaration(QB::DesignEntity::IF, syn2)});
        //todo: clarify if ptr to declarations of the query are in each such that clause?
        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS, syn1, syn2, declarations);
        Table result = dataPreprocessor->getTableByRelation(suchThatClause);
        cout << "finish" << endl;

    }
}