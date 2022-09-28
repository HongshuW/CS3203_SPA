//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_CLAUSEVISITOR_H
#define SPA_CLAUSEVISITOR_H

#include "pkb/table/Table.h"
#include "query_builder/clauses/PatternClause.h"
#include "query_builder/clauses/SelectClause.h"
#include "query_builder/clauses/WithClauses.h"
#include "query_builder/clauses/SuchThatClause.h"
#include "DataPreprocessor.h"
#include <iostream>
#include <variant>

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

namespace QE {
    using Clause = std::variant<
            shared_ptr<PatternClause>,
            shared_ptr<WithClause>,
            shared_ptr<SuchThatClause>> ;
    typedef  shared_ptr<vector<Declaration>> Declarations;

    class ClauseVisitor {
        shared_ptr<DataPreprocessor> dataPreprocessor;
        Declarations declarations;
    public:
        explicit ClauseVisitor(shared_ptr<DataPreprocessor> dataPreprocessor, Declarations declarations);
        Table operator()(shared_ptr<PatternClause> patternClause);
        Table operator()(shared_ptr<SuchThatClause> suchThatClause);
        Table operator()(shared_ptr<WithClause> withClause);
    };

} // QE

#endif //SPA_CLAUSEVISITOR_H
