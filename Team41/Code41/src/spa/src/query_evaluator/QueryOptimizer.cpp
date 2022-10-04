//
// Created by Nafour on 4/10/22.
//

#include "QueryOptimizer.h"
#include <set>

namespace QE {
    const int  QueryOptimizer::NO_SYN_GROUP_IDX = -1;

    QueryOptimizer::QueryOptimizer(shared_ptr<Query> query): query(query) {
        initMaps();
        initParent();

        initEdges();
        processEdges();
    }

    ConnectedClauseGroups QueryOptimizer::optimise() {
        return optimiseSubGroups(getConnectedGroups());
    }

    void QueryOptimizer::initParent() {
        parent = vector<int>(clauseCount);
        for (int i = 0; i < clauseCount; i++) {
            parent[i] = i;
        }
    }

    void QueryOptimizer::initMaps() {
        int clauseId = 0;
        for (auto clause: *query->suchThatClauses) {
            clauseIdMap.insert({clause, clauseId});
            idClauseMap.insert({clauseId, clause});
            clauseId++;
        }
        for (auto clause: *query->patternClauses) {
            clauseIdMap.insert({clause, clauseId});
            idClauseMap.insert({clauseId, clause});
            clauseId++;
        }
        for (auto clause: *query->withClauses) {
            clauseIdMap.insert({clause, clauseId});
            idClauseMap.insert({clauseId, clause});
            clauseId++;
        }
        clauseCount = clauseIdMap.size();
    }

    void QueryOptimizer::initEdges() {
        size_t M = clauseIdMap.size();
        const bool value = false;
        bool visited[M][M];
        std::fill(*visited, *visited + M*M, value);

        for (auto& it_i: clauseIdMap) {
            auto curr_i = it_i.first;
            for (auto& it_j: clauseIdMap) {
                auto curr_j = it_j.first;
                if (curr_i != curr_j
                && hasCommonSyn(curr_i, curr_j)
                && !visited[clauseIdMap.at(curr_i)][clauseIdMap.at(curr_j)]) {
                    visited[clauseIdMap.at(curr_i)][clauseIdMap.at(curr_j)] = true;
                    edges.emplace_back(clauseIdMap.at(curr_i), clauseIdMap.at(curr_j));
                }
            }
        }
    }

    bool QueryOptimizer::hasCommonSyn(shared_ptr<ConditionalClause> clause1, shared_ptr<ConditionalClause> clause2) {

        auto set1 = clause1->getSynonymNames();
        for (const auto& syn: clause2->getSynonymNames()) {
            if (set1.count(syn)) return true;
        }
        return false;
    }

    int QueryOptimizer::root(int clause) {
        if (clause == parent[clause]) {
            return clause;
        }
        return parent[clause] = root(parent[clause]);
    }

    int QueryOptimizer::connect(int clause1, int clause2) {
        clause1 = root(clause1);
        clause2 = root(clause2);
        if (clause1 != clause2) { parent[clause2] = clause1; }
    }

    void QueryOptimizer::processEdges() {
        for (auto edge: edges) {
            connect(edge.first, edge.second);
        }
    }

    void QueryOptimizer::connectedComponents() {
        set<int> s;

        // Traverse all vertices
        for (int i = 0; i < clauseCount; i++) {

            // Insert all topmost
            // vertices obtained
            s.insert(root(parent[i]));
        }

        // Print count of connected components
        cout << s.size() << '\n';
    }

    ConnectedClauseGroups QueryOptimizer::getConnectedGroups() {
        ConnectedClauseGroups ccg = make_shared<unordered_map<int, shared_ptr<vector<shared_ptr<ConditionalClause>>>>>();
        ccg->insert({NO_SYN_GROUP_IDX, make_shared<vector<shared_ptr<ConditionalClause>>>()});

        for (int i = 0; i < clauseCount; i++) {
            //clauses without synonyms are grouped together
            if (idClauseMap.at(i)->getSynonymNames().size() == 0) {
                ccg->at(NO_SYN_GROUP_IDX)->push_back(idClauseMap.at(i));
                continue;
            }

            int currRoot = root(parent[i]);
            if (ccg->find(currRoot) == ccg->end()) {
                ccg->insert({currRoot, make_shared<vector<shared_ptr<ConditionalClause>>>()});
            }


            ccg->at(currRoot)->push_back(idClauseMap.at(i));
        }

        return ccg;
    }

    ConnectedClauseGroups QueryOptimizer::optimiseSubGroups(ConnectedClauseGroups ccg) {
        struct compareSynCount {
            inline bool operator() (shared_ptr<ConditionalClause> clause1, shared_ptr<ConditionalClause> clause2)
            {
                return clause1->getSynonymNames().size() < clause2->getSynonymNames().size();
            }
        };

        //sort
        for (auto it: *ccg) {
            auto vec = it.second;
            sort(vec->begin(), vec->end(), compareSynCount());
        }
        return ccg;
    }





} // QE