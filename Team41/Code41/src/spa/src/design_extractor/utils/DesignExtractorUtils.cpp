//
// Created by Xingchen Lin on 26/10/22.
//

#include "DesignExtractorUtils.h"

namespace DE {
		DesignExtractorUtils::DesignExtractorUtils() {}

		unordered_map<string, vector<shared_ptr<CallNode>>>
		DesignExtractorUtils::extractCallNodesFromProcedures(shared_ptr<AST::ProgramNode> rootPtr) {
			auto mapCallNodesToProcedures =
							unordered_map<string, vector<shared_ptr<CallNode>>>();
			vector<shared_ptr<ProcedureNode>> procedureList = rootPtr->procedureList;
			for (auto procedureNode: procedureList) {
				string name = procedureNode->procedureName;
				auto listOfCallNodes = vector<shared_ptr<CallNode>>();
				queue<vector<shared_ptr<StmtNode>>> queue;
				queue.push(procedureNode->stmtList);
				while (!queue.empty()) {
					auto stmtList = queue.front();
					queue.pop();
					for (auto stmtNode: stmtList) {
						NodeType nodeType = ASTUtils::getNodeType(stmtNode);
						switch (nodeType) {
							case AST::CALL_NODE: {
								shared_ptr<CallNode> callNode =
												dynamic_pointer_cast<CallNode>(stmtNode);
								listOfCallNodes.push_back(callNode);
								break;
							}
							case AST::IF_NODE: {
								shared_ptr<IfNode> ifNode = dynamic_pointer_cast<IfNode>(stmtNode);
								vector<shared_ptr<StmtNode>> ifStmtList = ifNode->ifStmtList;
								vector<shared_ptr<StmtNode>> elseStmtList = ifNode->elseStmtList;
								queue.push(ifStmtList);
								queue.push(elseStmtList);
								break;
							}
							case AST::WHILE_NODE: {
								shared_ptr<WhileNode> whileNode =
												dynamic_pointer_cast<WhileNode>(stmtNode);
								vector<shared_ptr<StmtNode>> whileStmtList = whileNode->stmtList;
								queue.push(whileStmtList);
								break;
							}
							default:
								break;
						}
					}
				}
				if (!listOfCallNodes.empty()) {
					mapCallNodesToProcedures.insert(make_pair(name, listOfCallNodes));
				}
			}
			return mapCallNodesToProcedures;
		}

		void DesignExtractorUtils::extractVariablesFromCallNodesInProceduresToList(shared_ptr<AST::CallNode> callNode,
																																							 unordered_map<std::string, unordered_set<std::string>> mappedProceduresToVar,
																																							 unordered_map<std::string, vector<shared_ptr<AST::CallNode>>> mappedCallNodesToProcedures,
																																							 unordered_set<std::string> &uniqueVarList) {
			queue<shared_ptr<CallNode>> queue;
			queue.push(callNode);
			while (!queue.empty()) {
				auto callNodeEntry = queue.front();
				queue.pop();
				unordered_set<string> usedVarList = unordered_set<string>();
				if (mappedProceduresToVar.count(callNodeEntry->procedureName)) {
					usedVarList = mappedProceduresToVar.at(callNodeEntry->procedureName);
				}

				uniqueVarList.insert(usedVarList.begin(), usedVarList.end());

				if (mappedCallNodesToProcedures.count(callNodeEntry->procedureName) != 0) {
					auto otherCallNodes =
									mappedCallNodesToProcedures.at(callNodeEntry->procedureName);
					for (auto n: otherCallNodes) {
						queue.push(n);
					}
				}
			}

		}

		vector<shared_ptr<StmtNode>>
		DesignExtractorUtils::extractIfAndWhileNodesFromProcedures(shared_ptr<AST::ProgramNode> rootPtr) {
			vector<shared_ptr<StmtNode>> extractedNodes;
			for (auto procedureNode : rootPtr->procedureList) {
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
								extractedNodes.push_back(ifNode);
								queue.push(ifNode->ifStmtList);
								queue.push(ifNode->elseStmtList);
								break;
							}
							case AST::WHILE_NODE: {
								shared_ptr<WhileNode> whileNode =
												dynamic_pointer_cast<WhileNode>(stmtNode);
								extractedNodes.push_back(whileNode);
								queue.push(whileNode->stmtList);
								break;
							}
							default:
								break;
						}
					}
				}
			}

			return extractedNodes;
		}
} // DE