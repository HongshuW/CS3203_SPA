//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_SELECTCLAUSE_H
#define SPA_SELECTCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/AttrRef.h"
#include "Clause.h"
#include <ostream>
#include <variant>
#include <vector>

using namespace std;

namespace QE {
    class ClauseEvaluator;
}

namespace QB {
    enum class ReturnType {
        TUPLE,
        BOOLEAN
    };

    using Elem = variant<Synonym, AttrRef>;

    class SelectClause : public Clause {
    public:
        //! Can be Synonym, Tuple or BOOLEAN
        ReturnType returnType;
        //! To store results for single synonym and tuple only;
        shared_ptr<vector<Elem>> returnResults;
        //! For BOOLEAN
        SelectClause(ReturnType returnType);
        //! For Tuple
        SelectClause(ReturnType returnType, shared_ptr<vector<Elem>> returnResults);

        bool isBoolean() const;
        bool isTuple() const;

        static const int ELEM_SYN_IDX = 0;
        static const int ELEM_ATTR_REF_IDX = 1;

        bool operator==(const Clause& clause) const override;
        ostream& print(ostream& os) const override;
        Table accept(shared_ptr<QE::ClauseEvaluator> clauseEvaluator) override;
    };
}

#endif //SPA_SELECTCLAUSE_H
