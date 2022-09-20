////
//// Created by Nafour on 4/9/22.
////
//
//#ifndef SPA_DUMMYQUERYBUILDER_H
//#define SPA_DUMMYQUERYBUILDER_H
//
//#include "query_builder/QueryBuilder.h"
//using namespace QB;
//namespace TestQE {
//
//class DummyQueryBuilder {
//    vector<Declaration> declarations = vector<Declaration>{};
//    shared_ptr<SelectClause> selectClause = make_shared<SelectClause>(SelectClause(Synonym("dummySyn"))) ;
//    vector<shared_ptr<SuchThatClause>> suchThatClauses = vector<shared_ptr<SuchThatClause>>();
//public:
//    DummyQueryBuilder * addDeclaration(const Declaration& declaration);
//    DummyQueryBuilder * addDeclarations(shared_ptr<vector<Declaration>> declarations);
//    DummyQueryBuilder * addSelectClause(shared_ptr<SelectClause> selectClause1);
//    DummyQueryBuilder * addSuchThatClause(shared_ptr<SuchThatClause> suchThatClause);
//
//    shared_ptr<Query> buildPQLQuery();
//
//    DummyQueryBuilder();
//};
//
//} // TestQE
//
//#endif //SPA_DUMMYQUERYBUILDER_H
