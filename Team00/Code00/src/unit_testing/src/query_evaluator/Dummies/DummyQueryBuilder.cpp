//
// Created by Nafour on 4/9/22.
//

#include "DummyQueryBuilder.h"

#include <utility>

namespace TestQE {

    Query * DummyQueryBuilder::buildPQLQuery() {
        auto query = new Query();
        query->selectClause = this->selectClause;
        for (const auto& d: this-> declarations) {
            query->declarations->push_back(d);
        }
        return query;
    }

    DummyQueryBuilder * DummyQueryBuilder::addDeclaration(const Declaration& declaration) {
        this->declarations.push_back(declaration);
        return this;
    }

    DummyQueryBuilder::DummyQueryBuilder() {

    }

    DummyQueryBuilder *DummyQueryBuilder::addSelectClause(SelectClause * selectClause1) {
        this->selectClause = selectClause1;
        return this;
    }

} // TestQE