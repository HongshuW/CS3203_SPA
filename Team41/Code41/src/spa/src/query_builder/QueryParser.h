//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_QUERYPARSER_H
#define SPA_QUERYPARSER_H

#include "query_builder/commons/Query.h"
#include "query_builder/commons/Ref.h"
#include "utils/ExprNodeParser.h"
#include "utils/Utils.h"
#include "query_builder/commons/ExprStringTokenizer.h"
#include "query_builder/clauses/WithClauses.h"

using namespace std;

namespace QB {
    class QueryParser {
    private:
        shared_ptr<Query> query;
        unsigned int currIdx;
        vector<string> tokens;
        string peek();
        string previous();
        string pop();
        bool isWithinBound();
        bool expect(string s);
        bool match(string s);
        bool parseDeclarations();
        void parseSelectClause();
        Ref parseRef();
        void parseSuchThatClause();
        bool parseSuchThat();
        bool parsePattern();
        void parsePatternClause();
        ExpressionSpec parseExpressionSpec();
        void parseBooleanSelectClause();
        Elem parseTupleSelectClause();
        void parseWithClause();
        bool parseWith();
        WithRef parseWithRef();

    public:
        //! Parse tokens to Query object, throw Syntax Error if encounter
        explicit QueryParser(std::vector<std::string> tokens);

        shared_ptr<Query> parse();
    };
}

#endif //SPA_QUERYPARSER_H