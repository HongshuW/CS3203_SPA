//
// Created by Nafour on 29/9/22.
//

#ifndef SPA_CLAUSEVISITORCONSTANTS_H
#define SPA_CLAUSEVISITORCONSTANTS_H
#include <string>
#include "pkb/table/Table.h"

using namespace std;
namespace QE {

    class ClauseVisitorConstants {


    public:
        static const string BOOL_RESULT_HEADER;
        static const string TRUE;
        static const string FALSE;
        static const Table TRUE_TABLE;
        static const Table FALSE_TABLE;

    };

} // QE

#endif //SPA_CLAUSEVISITORCONSTANTS_H
