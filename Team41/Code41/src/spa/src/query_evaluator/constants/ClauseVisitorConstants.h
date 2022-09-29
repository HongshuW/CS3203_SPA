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

        static const int STR_VAL_IDX = 0;
        static const int INT_VAL_IDX = 1;
        static const int VECTOR_INT_VAL_IDX = 2;
        static const int VECTOR_STR_VAL_IDX = 3;

    };

} // QE

#endif //SPA_CLAUSEVISITORCONSTANTS_H
