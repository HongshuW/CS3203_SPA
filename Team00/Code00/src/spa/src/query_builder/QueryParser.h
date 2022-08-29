//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_QUERYPARSER_H
#define SPA_QUERYPARSER_H

#include "query_builder/commons/Query.h"
#include "query_builder/commons/Ref.h"

namespace QB {
    class QueryParser {
    private:
        Query* query;
        unsigned int currIdx;
        std::vector<std::string> tokens;
        std::string peek();
        std::string pop();
        bool expect(std::string s);
        bool match(std::string s);
        bool parseDeclarations();
        bool parseSelectClause();
        Ref parseRef();
        bool isDigit(const std::string &str);
        bool parseFollowsClause();
        bool parseFollowsTClause();
        bool parseSuchThatClause();
        bool parsePatternClause();

    public:
        explicit QueryParser(std::vector<std::string> tokens);

        Query parse();
    };
}

#endif //SPA_QUERYPARSER_H