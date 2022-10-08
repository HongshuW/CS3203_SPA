//
// Created by Xingchen Lin on 8/10/22.
//

#ifndef SPA_VALIDATORCONSTANTS_H
#define SPA_VALIDATORCONSTANTS_H

#include <string>

using namespace std;

namespace SourceParser {

    class ValidatorConstants {
    public:
        static const string SP_AST_EXCEPTION_CYCLIC_DEPENDENCY;
        static const string SP_AST_EXCEPTION_PROCEDURE_NOT_FOUND;
        static const string SP_AST_EXCEPTION_PROCEDURE_CALLS_ITSELF;
        static const string SP_AST_EXCEPTION_DUPLICATE_PROCEDURE;
    };

} // SourceParser

#endif //SPA_VALIDATORCONSTANTS_H
