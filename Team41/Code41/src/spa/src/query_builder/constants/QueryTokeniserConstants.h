//
// Created by Nafour on 27/9/22.
//

#ifndef SPA_QUERYTOKENISERCONSTANTS_H
#define SPA_QUERYTOKENISERCONSTANTS_H
#include <string>

using namespace std;
namespace QB {

    class QueryTokeniserConstants {
    public:
        static const string STMT;
        static const string EMPTY_STR;
        static const string DOUBLE_QUOTE;


        static const char HASH;
        static const char STAR;
        static const char DOUBLE_QUOTE_CHAR;

        static const string PQL_TOKENISE_EXCEPTION_UNEXPECTED_TOKEN;
    };

} // QB

#endif //SPA_QUERYTOKENISERCONSTANTS_H
