//
// Created by hongshu wang on 12/9/22.
//

#ifndef SPA_PATTERNTABLE_H
#define SPA_PATTERNTABLE_H

#include "AST/ExprNode.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "Table.h"

using namespace QB;

class PatternTable : public Table {
private:
    bool isSubExpression(shared_ptr<ExprNode> pattern, shared_ptr<ExprNode> queriedPattern);
    bool isSameExpression(shared_ptr<ExprNode> pattern, shared_ptr<ExprNode> queriedPattern);
public:
    // header and rows only record meta information about a pattern, a pattern AST is stored as a pointer
    vector<shared_ptr<ExprNode>> patternColumn;

    explicit PatternTable();
    shared_ptr<Table> getMatchedPatterns(ExpressionSpec expressionSpec);
    void addPattern(vector<string> metainfo, shared_ptr<ExprNode> pattern);
};


#endif //SPA_PATTERNTABLE_H
