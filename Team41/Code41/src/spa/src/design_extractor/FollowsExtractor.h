//
// Created by Aaron on 10/9/22.
//

#ifndef SPA_FOLLOWSEXTRACTOR_H
#define SPA_FOLLOWSEXTRACTOR_H

#include "DesignExtractor.h"

using namespace DE;

class FollowsExtractor {
public:
    shared_ptr<list<vector<string>>> extractFollows(shared_ptr<ProgramNode> programNode);
    shared_ptr<list<vector<string>>> extractFollowsStar(shared_ptr<AST::ProgramNode> programNode);
private:
    static vector<vector<shared_ptr<StmtNode>>> getListOfStmtList(shared_ptr<ProgramNode> programNode);
    static void saveFollowsToOutput(vector<shared_ptr<StmtNode>> stmtList,
                             list<vector<string>>* output,
                                    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
                                    nodeToLineNoMap);

    static void saveFollowsStarToOutput(vector<shared_ptr<StmtNode>> stmtList,
                                    list<vector<string>>* output,
                                    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>>
                                    nodeToLineNoMap);
};


#endif //SPA_FOLLOWSEXTRACTOR_H
