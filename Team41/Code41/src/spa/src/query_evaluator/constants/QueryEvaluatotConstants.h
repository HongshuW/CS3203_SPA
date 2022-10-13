//
// Created by Nafour on 12/10/22.
//

#ifndef SPA_QUERYEVALUATOTCONSTANTS_H
#define SPA_QUERYEVALUATOTCONSTANTS_H
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/AttrRef.h"

namespace QE {

    class QueryEvaluatotConstants {
    public:
        static const vector<DesignEntity> entitiesToFilter;
        static const vector<AttrName> attrsToFilter;
    };

} // QE

#endif //SPA_QUERYEVALUATOTCONSTANTS_H
