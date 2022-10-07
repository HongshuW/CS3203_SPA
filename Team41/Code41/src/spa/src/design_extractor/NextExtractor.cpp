//
// Created by Aaron on 1/10/22.
//

#include "NextExtractor.h"
#include "../AST/utils/ASTUtils.h"
#include "../CFG/CFG.h"

shared_ptr<list<vector<string>>> NextExtractor::extractNext(shared_ptr<ProgramNode> programNode) {
    vector<shared_ptr<ProcedureNode>> procedureList = programNode->procedureList;
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    auto firstLineNumToProcMap = ASTUtils::getFirstLineNumToProcMap(programNode);
    list<vector<string>> output;
    for (auto procedure : procedureList) {
        int startNum = firstLineNumToProcMap->at(procedure);
        CFG cfg = CFG(*procedure, stmtNumbers);
        int cfgSize = cfg.cfg->size() + startNum;
        for (int i = startNum; i < cfgSize; i++) {
            unordered_set<int> children = cfg.cfg->find(i)->second;
            for (int c : children) {
                vector<string> nextEntry;
                nextEntry.push_back(to_string(i));
                nextEntry.push_back(to_string(c));
                output.push_back(nextEntry);
            }
        }
    }

    return make_shared<list<vector<string>>>(output);
}

vector<string> NextExtractor::extractNextStarWithEndOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    auto ans = vector<string>();
    auto firstLineNumToProcMap = ASTUtils::getFirstLineNumToProcMap(programNode);
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);

    int end = args.getEndStmtNo();

    bool invalidEndArg = stmtNoToProcMap->count(end) == 0 && end != 0;
    if (invalidEndArg) {
        return {};
    }

    shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(end);
    CFG cfg = CFG(*procedureNode, stmtNumbers);
    int startNum = firstLineNumToProcMap->at(procedureNode);
    int endNum = cfg.cfg->size() + startNum;

    for (int i = startNum; i < endNum; i++) {
        StmtNoArgs testArgs;
        testArgs.setStartAndEndStmtNo(i, end);
        vector<string> output = NextExtractor::extractNextStarWithStartAndEnd(programNode, testArgs);
        if (!output.empty()) {
            ans.push_back(to_string(i));
        }
    }

    return ans;
}

vector<string> NextExtractor::extractNextStarWithStartOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    auto ans = vector<string>();
    auto firstLineNumToProcMap = ASTUtils::getFirstLineNumToProcMap(programNode);
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);

    int start = args.getStartStmtNo();

    bool invalidStartArg = stmtNoToProcMap->count(start) == 0 && start != 0;
    if (invalidStartArg) {
        return {};
    }

    shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(start);
    CFG cfg = CFG(*procedureNode, stmtNumbers);
    int startNum = firstLineNumToProcMap->at(procedureNode);
    int endNum = cfg.cfg->size() + startNum;

    for (int i = startNum; i < endNum; i++) {
        StmtNoArgs testArgs;
        testArgs.setStartAndEndStmtNo(start, i);
        vector<string> output = NextExtractor::extractNextStarWithStartAndEnd(programNode, testArgs);
        if (!output.empty()) {
            ans.push_back(to_string(i));
        }
    }

    return ans;
}

vector<string> NextExtractor::extractNextStarWithStartAndEnd(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    int start = args.getStartStmtNo();
    int end = args.getEndStmtNo();

    auto ans = vector<string>();
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);

    if (!NextExtractor::areBothArgsVaild(programNode, start, end)) {
        return {};
    }

    shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(start);
    CFG cfg = CFG(*procedureNode, stmtNumbers);
    int visitedArrSize = cfg.cfg->size() + start + 1;
    vector<bool> visitedArr;
    for (int i = 0; i < visitedArrSize; i++) {
        visitedArr.push_back(false);
    }

    NextExtractor::extractNextStarWithStartAndEndDFSHelper(start, end, cfg, ans, visitedArr);
    if (!ans.empty()) {
        ans = vector<string>();
        ans.push_back(to_string(start));
        ans.push_back(to_string(end));
    }
    return ans;
}

void NextExtractor::extractNextStarWithStartAndEndDFSHelper(int start, int end, CFG cfg, vector<string>& ans, vector<bool>& visitedArr) {
    unordered_set<int> children = cfg.cfg->find(start)->second;
    if (!children.empty()) {
        for (int stmtNo : children) {
            if (stmtNo == end) {
                ans.push_back(to_string(end));
                return;
            } 
            else if (stmtNo > start) {
                extractNextStarWithStartAndEndDFSHelper(stmtNo, end, cfg, ans, visitedArr);
            }

            else if (stmtNo <= start) {
                if (!visitedArr.at(stmtNo)) {
                    visitedArr.at(stmtNo) = true;
                    extractNextStarWithStartAndEndDFSHelper(stmtNo, end, cfg, ans, visitedArr);
                }  
            }
        }
    }
}

bool NextExtractor::areBothArgsVaild(shared_ptr<ProgramNode> programNode, int start, int end) {
    auto stmtNoToProcMap = ASTUtils::getLineNumToProcMap(programNode);

    bool invalidStartArg = stmtNoToProcMap->count(start) == 0 && start != 0;
    bool invalidEndArg = stmtNoToProcMap->count(end) == 0 && end != 0;

    if (invalidStartArg || invalidEndArg) {
        return false;
    }
    else {
        if (stmtNoToProcMap->count(start) != 0 && stmtNoToProcMap->count(end) != 0) {
            bool startAndEndSameProcedure = stmtNoToProcMap->at(start) == stmtNoToProcMap->at(end);
            if (!startAndEndSameProcedure) {
                return false;
            }
        }
    }
    return true;
}

vector<string> NextExtractor::extractNextStar(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    int start = args.getStartStmtNo();
    int end = args.getEndStmtNo();

    if (args.startAndEndExists()) {
        return extractNextStarWithStartAndEnd(programNode, args);
    }

    if (args.startExistsOnly()) {
        return extractNextStarWithStartOnly(programNode, args);
    }

    if (args.endExistsOnly()) {
        return extractNextStarWithEndOnly(programNode, args);
    }

    vector<string> ans;
    return ans;
}

