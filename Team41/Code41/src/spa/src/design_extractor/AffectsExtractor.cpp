//
// Created by Aaron on 1/10/22.
//
#include <queue>
#include <algorithm>
#include "AffectsExtractor.h"
#include "EntityExtractor.h"
#include "ModifiesExtractor.h"
#include "AST/ProcedureNode.h"

using namespace DE;

vector<string> AffectsExtractor::extractAffects(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    if (args.startAndEndExists()) {
        return extractAffectsWithStartAndEnd(programNode, args);
    }

    if (args.startExistsOnly()) {
        return extractAffectsWithStartOnly(programNode, args);
    }

    if (args.endExistsOnly()) {
        return extractAffectsWithEndOnly(programNode, args);
    }
    vector<string> ans;
    return ans;
}

vector<string> AffectsExtractor::extractAffectsStar(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    if (args.startAndEndExists()) {
        return extractAffectsStarWithStartAndEnd(programNode, args);
    }

    if (args.startExistsOnly()) {
        return extractAffectsStarWithStartOnly(programNode, args);
    }

    if (args.endExistsOnly()) {
        return extractAffectsStarWithEndOnly(programNode, args);
    }
    vector<string> ans;
    return ans;
}

vector<string> AffectsExtractor::extractAffectsWithStartAndEnd(shared_ptr<ProgramNode> programNode,
                                                               StmtNoArgs args) {
    int startStmtNo = args.getStartStmtNo();
    int endStmtNo = args.getEndStmtNo();

    if (!AffectsExtractor::areBothArgsVaild(programNode, startStmtNo, endStmtNo)) {
        return {};
    }

    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> stmtNoToProcMap
        = ASTUtils::getLineNumToProcMap(programNode);
    auto stmtNoToAssignReadAndCallMap 
        = AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(programNode);

    vector<string> ans;

    auto startNode = dynamic_pointer_cast<AssignNode>(stmtNoToAssignReadAndCallMap.at(to_string(startStmtNo)));
    auto endNode = dynamic_pointer_cast<AssignNode>(stmtNoToAssignReadAndCallMap.at(to_string(endStmtNo)));
   
    // check if variable modified in startNode is used in endNode
    string modifiedVar = startNode->variableNode->variable;
    unordered_set<string> usedVars = EntityExtractor::getVariablesFromExprNode(endNode->expressionNode);
    if (usedVars.count(modifiedVar) == 0) {
        return {};
    }

    shared_ptr<ProcedureNode> procedureNode = stmtNoToProcMap->at(startStmtNo);
    string procedureName = procedureNode->procedureName;
    CFG cfg = CFG(*procedureNode, stmtNumbers);
    unordered_set<int> children = cfg.cfg->find(startStmtNo)->second;

    int visitedArrSize = cfg.cfg->size() + startStmtNo + 1;
    vector<bool> visitedArr;
    for (int i = 0; i < visitedArrSize; i++) {
        visitedArr.push_back(false);
    }
    unordered_set<string> uniqueStmtNoList;
    list<vector<string>> validPathsList;
    vector<string> validPaths;
    AffectsExtractor::extractAffectsWithStartAndEndDFSHelper(startStmtNo, endStmtNo, cfg,
        validPathsList, validPaths, visitedArr);

    // can't reach end from start, hence invalid
    if (validPathsList.empty()) {
        return {};
    }

    // handle visited nodes from paths
    for (auto path: validPathsList) {
        unordered_set<string> filteredStmtNoList;
        for (auto var : path) {
            if (var != to_string(startStmtNo)) {
                if (stmtNoToAssignReadAndCallMap.count(var) != 0) {
                    filteredStmtNoList.insert(var);
                }
            }
        }

        // all visited nodes are not assign, read or call nodes
        // hence path is vaild as variable not modified
        if (filteredStmtNoList.empty()) {
            ans.push_back(to_string(startStmtNo));
            ans.push_back(to_string(endStmtNo));
            return ans;
        }

        if (!isVarModified(modifiedVar, programNode, filteredStmtNoList)) {
            ans.push_back(to_string(startStmtNo));
            ans.push_back(to_string(endStmtNo));
            return ans;
        }
    }

    return ans;
}

vector<string> AffectsExtractor::extractAffectsWithEndOnly(shared_ptr<ProgramNode> programNode,
                                                           StmtNoArgs args) {
    vector<string> ans;
    int end = args.getEndStmtNo();
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> stmtNoToProcMap
        = ASTUtils::getLineNumToProcMap(programNode);

    bool stmtNoExistsInProg = stmtNoToProcMap->count(end) != 0;
    if (!stmtNoExistsInProg) {
        return {};
    }

    unordered_set<string> allStmtNoOfAssignNodes = AffectsExtractor::getAllStmtNoOfAssignNodes(end, programNode);
    bool isEndStmtNoAssignNode = allStmtNoOfAssignNodes.count(to_string(end));
    if (!isEndStmtNoAssignNode) {
        return {};
    }

    for (string stmtNo : allStmtNoOfAssignNodes) {
        int start = stoi(stmtNo);
        StmtNoArgs newArgs;
        newArgs.setStartAndEndStmtNo(start, end);
        vector<string> output = AffectsExtractor::extractAffectsWithStartAndEnd(programNode, newArgs);
        if (!output.empty()) {
            ans.push_back(stmtNo);
        }
    }

    return ans;
}

vector<string> AffectsExtractor::extractAffectsWithStartOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    vector<string> ans;
    int start = args.getStartStmtNo();
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> stmtNoToProcMap
        = ASTUtils::getLineNumToProcMap(programNode);

    bool stmtNoExistsInProg = stmtNoToProcMap->count(start) != 0;
    if (!stmtNoExistsInProg) {
        return {};
    }

    unordered_set<string> allStmtNoOfAssignNodes = AffectsExtractor::getAllStmtNoOfAssignNodes(start, programNode);
    bool isStartStmtNoAssignNode = allStmtNoOfAssignNodes.count(to_string(start));
    if (!isStartStmtNoAssignNode) {
        return {};
    }

    for (string stmtNo : allStmtNoOfAssignNodes) {
        int end = stoi(stmtNo);
        StmtNoArgs newArgs;
        newArgs.setStartAndEndStmtNo(start, end);
        vector<string> output = AffectsExtractor::extractAffectsWithStartAndEnd(programNode, newArgs);
        if (!output.empty()) {
            ans.push_back(stmtNo);
        }
    }

    return ans;
}

void AffectsExtractor::extractAffectsWithStartAndEndDFSHelper(int start, int end, CFG cfg, 
    list<vector<string>>& validPathsList, vector<string>& validPaths, vector<bool>& visitedArr) {
    unordered_set<int> children = cfg.cfg->find(start)->second;
    validPaths.push_back(to_string(start));

    if (!children.empty()) {
        for (int stmtNo : children) {
            if (stmtNo == end) {
                validPathsList.push_back(validPaths);
            }

            else if (stmtNo > start) {
                extractAffectsWithStartAndEndDFSHelper(stmtNo, end, cfg,
                    validPathsList, validPaths, visitedArr);
            }
           
            else if (stmtNo <= start) {
                if (!visitedArr.at(stmtNo)) {
                    visitedArr.at(stmtNo) = true;
                    extractAffectsWithStartAndEndDFSHelper(stmtNo, end, cfg,
                        validPathsList, validPaths, visitedArr);
                }
            }
        }
    }

    validPaths.pop_back();
}

vector<string> AffectsExtractor::extractAffectsStarWithStartAndEnd(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    int startStmtNo = args.getStartStmtNo();
    int endStmtNo = args.getEndStmtNo();

    if (!AffectsExtractor::areBothArgsVaild(programNode, startStmtNo, endStmtNo)) {
        return {};
    }

    // If Affects(a1, a2) is valid, Affects*(a1, a2) is also valid
    vector<string> output0 = AffectsExtractor::extractAffectsWithStartAndEnd(programNode, args);
    if (!output0.empty()) {
        return output0;
    }

    auto startArgsOnly = StmtNoArgs();
    startArgsOnly.setStartStmtNo(startStmtNo);

    auto endArgsOnly = StmtNoArgs();
    endArgsOnly.setEndStmtNo(endStmtNo);

    vector<string> output1 = AffectsExtractor::extractAffectsWithStartOnly(programNode, startArgsOnly);
    vector<string> output2 = AffectsExtractor::extractAffectsWithEndOnly(programNode, endArgsOnly);
    vector<string> output3;

    // for Affects*(a1, a2)
    // if there is an intersection between Affects(a1, _) and Affects(_, a2)
    // then Affects*(a1, a2) is valid

    sort(output1.begin(), output1.end());
    sort(output2.begin(), output2.end());

    set_intersection(output1.begin(), output1.end(),
        output2.begin(), output2.end(),
        back_inserter(output3));

    vector<string> ans;

    if (!output3.empty()) {
        ans.push_back(to_string(startStmtNo));
        ans.push_back(to_string(endStmtNo));
        return ans;
    }

    return {};
}

vector<string> AffectsExtractor::extractAffectsStarWithStartOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    vector<string> ans;
    int start = args.getStartStmtNo();
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> stmtNoToProcMap
        = ASTUtils::getLineNumToProcMap(programNode);

    bool stmtNoExistsInProg = stmtNoToProcMap->count(start) != 0;
    if (!stmtNoExistsInProg) {
        return {};
    }

    unordered_set<string> allStmtNoOfAssignNodes = AffectsExtractor::getAllStmtNoOfAssignNodes(start, programNode);
    bool isStartStmtNoAssignNode = allStmtNoOfAssignNodes.count(to_string(start));
    if (!isStartStmtNoAssignNode) {
        return {};
    }

    for (string stmtNo : allStmtNoOfAssignNodes) {
        int end = stoi(stmtNo);
        StmtNoArgs newArgs;
        newArgs.setStartAndEndStmtNo(start, end);
        vector<string> output = AffectsExtractor::extractAffectsStarWithStartAndEnd(programNode, newArgs);
        if (!output.empty()) {
            ans.push_back(stmtNo);
        }
    }

    return ans;
}

vector<string> AffectsExtractor::extractAffectsStarWithEndOnly(shared_ptr<ProgramNode> programNode, StmtNoArgs args) {
    vector<string> ans;
    int end = args.getEndStmtNo();
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> stmtNoToProcMap
        = ASTUtils::getLineNumToProcMap(programNode);

    bool stmtNoExistsInProg = stmtNoToProcMap->count(end) != 0;
    if (!stmtNoExistsInProg) {
        return {};
    }

    unordered_set<string> allStmtNoOfAssignNodes = AffectsExtractor::getAllStmtNoOfAssignNodes(end, programNode);
    bool isEndStmtNoAssignNode = allStmtNoOfAssignNodes.count(to_string(end));
    if (!isEndStmtNoAssignNode) {
        return {};
    }

    for (string stmtNo : allStmtNoOfAssignNodes) {
        int start = stoi(stmtNo);
        StmtNoArgs newArgs;
        newArgs.setStartAndEndStmtNo(start, end);
        vector<string> output = AffectsExtractor::extractAffectsStarWithStartAndEnd(programNode, newArgs);
        if (!output.empty()) {
            ans.push_back(stmtNo);
        }
    }

    return ans;
}

unordered_map<string, shared_ptr<StmtNode>> AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(shared_ptr<ProgramNode> programNode) {
    auto map = unordered_map<string, shared_ptr<StmtNode>>();
    shared_ptr<unordered_map<shared_ptr<StmtNode>, int>> stmtNumbers =
        ASTUtils::getNodePtrToLineNumMap(programNode);
    for (auto procedureNode : programNode->procedureList) {
        queue<vector<shared_ptr<StmtNode>>> queue;
        queue.push(procedureNode->stmtList);
        while (!queue.empty()) {
            auto stmtList = queue.front();
            queue.pop();
            for (auto stmtNode : stmtList) {
                NodeType nodeType = ASTUtils::getNodeType(stmtNode);
                switch (nodeType) {
                case AST::IF_NODE: {
                    shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
                    vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
                    vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
                    queue.push(ifStmtList);
                    queue.push(elseStmtList);
                    break;
                }
                case AST::WHILE_NODE: {
                    shared_ptr<WhileNode> whileNode = dynamic_pointer_cast<WhileNode>(stmtNode);
                    vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
                    queue.push(whileStmtList);
                    break;
                }

                case AST::ASSIGN_NODE: {
                    shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(stmtNode);
                    int stmtNo = stmtNumbers->at(assignNode);
                    map.insert({to_string(stmtNo), stmtNode});
                    break;
                }

                case AST::READ_NODE: {
                    shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(stmtNode);
                    int stmtNo = stmtNumbers->at(readNode);
                    map.insert({to_string(stmtNo), stmtNode });
                    break;
                }
                case AST::CALL_NODE: {
                    shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
                    int stmtNo = stmtNumbers->at(callNode);
                    map.insert({to_string(stmtNo), stmtNode });
                    break;
                }
                default:
                    break;
                }
            }
        }
    }

    return map;
}

unordered_set<string> AffectsExtractor::getAllStmtNoOfAssignNodes(int stmtNoArg, shared_ptr<ProgramNode> programNode) {
    unordered_set<string> stmtNoList;
    auto stmtNoToAssignReadAndCallMap
        = AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(programNode);
    for (auto ele : stmtNoToAssignReadAndCallMap) {
        string num = ele.first;
        shared_ptr<StmtNode> stmtNode = ele.second;
        NodeType type = ASTUtils::getNodeType(stmtNode);
        if (type == AST::ASSIGN_NODE) {
            stmtNoList.insert(num);
        }
    }

    return stmtNoList;
}

bool AffectsExtractor::isVarModified(string modifiedVar, 
    shared_ptr<ProgramNode> programNode, unordered_set<string> filteredStmtNoList) {
    auto stmtNoToAssignReadAndCallMap = AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(programNode);
    for (auto stmtNo : filteredStmtNoList) {
        auto stmtNode = stmtNoToAssignReadAndCallMap.at(stmtNo);
        NodeType nodeType = ASTUtils::getNodeType(stmtNode);
        switch (nodeType) {
        case AST::ASSIGN_NODE: {
            shared_ptr<AssignNode> assignNode = dynamic_pointer_cast<AssignNode>(stmtNode);
            string currentModifiedVar = assignNode->variableNode->variable;
            if (currentModifiedVar == modifiedVar) {
                return true;
            }
            break;
        }

        case AST::READ_NODE: {
            shared_ptr<ReadNode> readNode = dynamic_pointer_cast<ReadNode>(stmtNode);
            string currentModifiedVar = readNode->variableNode->variable;
            if (currentModifiedVar == modifiedVar) {
                return true;
            }
            break;
        }
        case AST::CALL_NODE: {
            shared_ptr<CallNode> callNode = dynamic_pointer_cast<CallNode>(stmtNode);
            auto mapProcToModifiedVar = ModifiesExtractor::mapProceduresToModifiedVariables(programNode);
            if (mapProcToModifiedVar.count(callNode->procedureName) != 0) {
                auto modifiedVarListFromProc = mapProcToModifiedVar.at(callNode->procedureName);
                for (auto v : modifiedVarListFromProc) {
                    if (v == modifiedVar) {
                        return true;
                    }
                }
            }
            break;
        }
        default:
            break;
        }
    }
    return false;
}

bool AffectsExtractor::areBothArgsVaild(shared_ptr<ProgramNode> programNode, int start, int end) {
    shared_ptr<unordered_map<int, shared_ptr<ProcedureNode>>> stmtNoToProcMap
    	= ASTUtils::getLineNumToProcMap(programNode);

    // both args must exist in program
    bool startStmtNoExistsInProg = stmtNoToProcMap->count(start) != 0;
    bool endStmtNoExistsInProg = stmtNoToProcMap->count(end) != 0;
    if (!startStmtNoExistsInProg || !endStmtNoExistsInProg) {
        return false;
    }
    // both args exists, check if they have the same procedure
    string startStmtNoProcName = stmtNoToProcMap->at(start)->procedureName;
    string endStmtNoProcName = stmtNoToProcMap->at(end)->procedureName;

    if (startStmtNoProcName != endStmtNoProcName) {
        return false;
    }

    //check if start and end are AssignNodes
    auto stmtNoToAssignReadAndCallMap = AffectsExtractor::getStmtNoToAssignReadAndCallNodesMap(programNode);

    bool startStmtNoIsAssignReadOrCall = stmtNoToAssignReadAndCallMap.count(to_string(start)) != 0;
    bool endStmtNoIsAssignReadOrCall = stmtNoToAssignReadAndCallMap.count(to_string(end)) != 0;
    if (!startStmtNoIsAssignReadOrCall || !endStmtNoIsAssignReadOrCall) {
        return false;
    }

    auto startStmtNode = stmtNoToAssignReadAndCallMap.at(to_string(start));
    auto endStmtNode = stmtNoToAssignReadAndCallMap.at(to_string(end));

    NodeType startNodeType = ASTUtils::getNodeType(startStmtNode);
    NodeType endNodeType = ASTUtils::getNodeType(endStmtNode);

    bool isStartStmtNodeAssign = startNodeType == AST::ASSIGN_NODE;
    bool isEndStmtNodeAssign = endNodeType == AST::ASSIGN_NODE;

    if (!isStartStmtNodeAssign || !isEndStmtNodeAssign) {
        return false;
    }

    return true;
}