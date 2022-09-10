//
// Created by Aaron on 10/9/22.
//

#ifndef SPA_FOLLOWSEXTRACTOR_H
#define SPA_FOLLOWSEXTRACTOR_H

#include "DesignExtractor.h"

using namespace DE;

class FollowsExtractor {
    static void extractFollows(shared_ptr<ProgramNode> programNode);
private:
    static void extractFollowsFromProcedureNode(shared_ptr<ProcedureNode> procedureNode);
    static void extractFollowsFromStmtList(vector<shared_ptr<StmtNode>> stmtList );
};


#endif //SPA_FOLLOWSEXTRACTOR_H
