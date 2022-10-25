//
// Created by Aaron on 22/10/22.
//

#ifndef SPA_CFGMAPPER_H
#define SPA_CFGMAPPER_H

#include <AST/ProgramNode.h>
#include <CFG/CFG.h>

class CFGMapper {
 public:
  CFGMapper(shared_ptr<ProgramNode> programNode);
  unordered_map<shared_ptr<ProcedureNode>, CFG> getProcNodeToCFGMap();

 private:
  shared_ptr<ProgramNode> programNode;
  unordered_map<shared_ptr<ProcedureNode>, CFG> procNodeToCFGMap;
  void initProcNodeToCFGMap();
};

#endif  // SPA_CFGMAPPER_H
