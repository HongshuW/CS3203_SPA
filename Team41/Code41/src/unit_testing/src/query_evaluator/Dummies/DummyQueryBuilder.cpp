//
// Created by Nafour on 4/9/22.
//

#include "DummyQueryBuilder.h"

#include <utility>

namespace TestQE {

    shared_ptr<Query> DummyQueryBuilder::buildPQLQuery() {
        auto query = make_shared<Query>();
        query->selectClause = this->selectClause;
        for (const auto& d: this-> declarations) {
            query->declarations->push_back(d);
        }
        for (const auto& clause: this->suchThatClauses) {
            query->suchThatClauses->push_back(clause);
        }

        return query;
    }

    DummyQueryBuilder * DummyQueryBuilder::addDeclaration(const Declaration& declaration) {
        this->declarations.push_back(declaration);
        return this;
    }

    DummyQueryBuilder::DummyQueryBuilder() {

    }

    DummyQueryBuilder *DummyQueryBuilder::addSelectClause(shared_ptr<SelectClause> selectClause1) {
        this->selectClause = selectClause1;
        return this;
    }

    DummyQueryBuilder *DummyQueryBuilder::addSuchThatClause(shared_ptr<SuchThatClause> suchThatClause) {
        this->suchThatClauses.push_back(suchThatClause);
        return this;
    }

    DummyQueryBuilder *DummyQueryBuilder::addDeclarations(shared_ptr<vector<Declaration>> declarations) {
        for (auto d: *declarations) {
            this->declarations.push_back(d);
        }
        return this;
    }

} // TestQE