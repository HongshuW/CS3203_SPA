//
// Created by hongshu wang on 12/9/22.
//

#ifndef SPA_PATTERNMANAGER_H
#define SPA_PATTERNMANAGER_H

#include "query_builder/commons/ExpressionSpec.h"
#include "table/PatternTable.h"

using namespace QB;

class PatternManager {
public:
    // getter
    static shared_ptr<Table> getMatchedPatterns(ExpressionSpec expressionSpec);
    // setter
    static void savePattern(vector<string> metainfo, shared_ptr<ExprNode> pattern);
private:
    static PatternTable patternTable;
};


#endif //SPA_PATTERNMANAGER_H
