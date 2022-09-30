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
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"
#include "utils/ErrorMessageFormatter.h"
#include "constants/QueryParserConstants.h"
#include "query_builder/clauses/pattern_clauses/AssignPatternClause.h"
#include "query_builder/clauses/pattern_clauses/IfPatternClause.h"
#include "query_builder/clauses/pattern_clauses/WhilePatternClause.h"
#include "query_builder/relations/Follows.h"
#include "query_builder/relations/FollowsT.h"
#include "query_builder/relations/Parent.h"
#include "query_builder/relations/ParentT.h"
#include "query_builder/relations/Uses.h"
#include "query_builder/relations/UsesP.h"
#include "query_builder/relations/UsesS.h"
#include "query_builder/relations/Modifies.h"
#include "query_builder/relations/ModifiesP.h"
#include "query_builder/relations/ModifiesS.h"
#include "query_builder/relations/Calls.h"
#include "query_builder/relations/CallsT.h"
#include "query_builder/relations/Next.h"
#include "query_builder/relations/NextT.h"
#include "query_builder/relations/Affects.h"
#include "query_builder/relations/AffectsT.h"
#include "query_builder/relations/SuchThatRelations.h"
#include <unordered_map>

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
        bool expect(const string& s);
        bool match(const string& s);
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