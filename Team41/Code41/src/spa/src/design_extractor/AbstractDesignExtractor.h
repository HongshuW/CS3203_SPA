//
// Created by Nafour on 26/10/22.
//

#ifndef SPA_ABSTRACTDESIGNEXTRACTOR_H
#define SPA_ABSTRACTDESIGNEXTRACTOR_H
#include "AST/ProgramNode.h"
#include "pkb/DataModifier.h"

namespace DE {

    class AbstractDesignExtractor {

    public:
        explicit AbstractDesignExtractor(shared_ptr<DataModifier> dataModifier, shared_ptr<ProgramNode> programNode);
        virtual void extract() = 0;
    protected:
        virtual void save() = 0;
        shared_ptr<DataModifier> dataModifier;
        shared_ptr<ProgramNode> programNode;
        shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers;
    };

} // DE

#endif //SPA_ABSTRACTDESIGNEXTRACTOR_H
