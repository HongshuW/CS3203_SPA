//
// Created by Aaron on 10/9/22.
//

#ifndef SPA_FOLLOWSEXTRACTOR_H
#define SPA_FOLLOWSEXTRACTOR_H

#include "DesignExtractor.h"

using namespace DE;

class FollowsExtractor {
public:
    static void extractFollows(shared_ptr<ProgramNode> programNode);
private:
      static vector<vector<shared_ptr<StmtNode>>> extractFollowsFromProcedureNode(shared_ptr<ProcedureNode> procedureNode);
      static void extractFollowsFromStmtList(vector<vector<shared_ptr<StmtNode>>> listOfStmtList,
                                             shared_ptr<ProgramNode> programNode);
      static void saveFollowsToPKB(vector<shared_ptr<StmtNode>> stmtList,
                                   shared_ptr<ProgramNode> programNode);
};


#endif //SPA_FOLLOWSEXTRACTOR_H
