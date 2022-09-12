//
// Created by hongshu wang on 12/9/22.
//

#ifndef SPA_PATTERNTABLE_H
#define SPA_PATTERNTABLE_H

#include "AST/ExprNode.h"
#include "Table.h"

class PatternTable : public Table {
public:
    // header and rows only record meta information about a pattern, a pattern AST is stored as a pointer
    vector<shared_ptr<ExprNode>> patternColumn;

    explicit PatternTable();
};


#endif //SPA_PATTERNTABLE_H
