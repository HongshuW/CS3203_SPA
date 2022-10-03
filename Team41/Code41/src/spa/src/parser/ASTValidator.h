//
// Created by Tan Wei En on 3/10/22.
//

#ifndef SPA_ASTVALIDATOR_H
#define SPA_ASTVALIDATOR_H

#include "AST/ProgramNode.h"
#include "unordered_map"

using namespace std;

namespace SourceParser {
    class ASTValidator {
    private:
        unordered_set<string> procedureNames;
        unordered_map<string, unordered_set<string>> procedureCalls;
        shared_ptr<ProgramNode> ast;
        void updateProcedureNames();
        bool validateCalls();
        bool validateLoops();
        bool calls(string procedure, string calledProcedure);

    public:
        /**
         * Validates an AST
         *
         * @param shared_ptr<ProgramNode> AST.
         * @return true if AST is valid, false otherwise
         */
        bool validateAST();
        explicit ASTValidator(shared_ptr<ProgramNode> ast);
    };
}



#endif //SPA_ASTVALIDATOR_H
