//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

#pragma once

#include <ostream>
#include "query_evaluator/ClauseEvaluator.h"

using namespace std;

namespace QE {
    class ClauseEvaluator;
}

namespace QB {
    class Clause {
    public:
        virtual ~Clause() = default;
        virtual bool operator==(const Clause& clause) const = 0;
        virtual ostream& print(ostream & os) const = 0;
        virtual Table accept(shared_ptr<QE::ClauseEvaluator> clauseEvaluator);
    };

} // QB

#endif //SPA_CLAUSE_H
