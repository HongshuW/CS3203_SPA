//
// Created by Nafour on 4/9/22.
//

#ifndef SPA_DUMMYQUERYBUILDER_H
#define SPA_DUMMYQUERYBUILDER_H

#include "query_builder/QueryBuilder.h"
using namespace QB;
namespace TestQE {

class DummyQueryBuilder {
    vector<Declaration> declarations = vector<Declaration>{};
    SelectClause * selectClause = new SelectClause(Synonym("dummySyn"));
    vector<SuchThatClause> suchThatClauses = vector<SuchThatClause>{};
public:
    DummyQueryBuilder * addDeclaration(const Declaration& declaration);
    DummyQueryBuilder * addSelectClause(SelectClause* selectClause1);

    Query * buildPQLQuery();

    DummyQueryBuilder();
};

} // TestQE

#endif //SPA_DUMMYQUERYBUILDER_H
