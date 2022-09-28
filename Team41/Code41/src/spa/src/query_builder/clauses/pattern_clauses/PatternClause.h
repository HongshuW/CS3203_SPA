//
// Created by Xingchen Lin on 11/9/22.
//

#ifndef SPA_PATTERNCLAUSE_H
#define SPA_PATTERNCLAUSE_H

#pragma once
#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Ref.h"

using namespace QB;

namespace QB {
    class PatternClause {
    public:
        Synonym arg1; // must be declared as If
        Ref arg2; // entRef -> synonym, _, ident
        PatternClause(Synonym arg1, Ref arg2);
    };
}

#endif //SPA_PATTERNCLAUSE_H
