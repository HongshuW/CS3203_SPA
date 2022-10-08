//
// Created by Nafour on 8/10/22.
//

#ifndef SPA_TESTQUERYBUILDER_H
#define SPA_TESTQUERYBUILDER_H
#include <vector>
#include "query_builder/clauses/such_that_clauses/SuchThatClause.h"
#include "query_builder/clauses/select_clauses/SelectClause.h"
#include "query_builder/clauses/pattern_clauses/PatternClause.h"
#include "query_builder/clauses/with_clauses/WithClauses.h"
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/Declaration.h"
#include "query_builder/commons/ExpressionSpec.h"
#include "query_builder/commons/Query.h"

namespace TestQE {

    class TestQueryBuilder: public enable_shared_from_this<TestQueryBuilder> {
        shared_ptr<vector<Declaration>> declarations;
        shared_ptr<SelectClause> selectClause;
        shared_ptr<vector<shared_ptr<SuchThatClause>>> suchThatClauses;
        shared_ptr<vector<shared_ptr<PatternClause>>> patternClauses;
        shared_ptr<vector<shared_ptr<WithClause>>> withClauses;

    public:
        TestQueryBuilder();
        shared_ptr<TestQueryBuilder> addDeclaration(DesignEntity designEntity, Synonym synonym);
        shared_ptr<TestQueryBuilder> setReturnBoolean();
        shared_ptr<TestQueryBuilder> setReturnTuple();
        shared_ptr<TestQueryBuilder> addToSelect(Elem elem);
        shared_ptr<TestQueryBuilder> addAffects(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addAffectsT(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addCalls(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addCallT(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addFollows(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addFollowsT(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addModifiesS(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addModifiesP(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addNext(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addNextT(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addParent(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addParentT(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addUsesS(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addUsesP(Ref arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addWith(WithRef arg1, WithRef arg2);
        shared_ptr<TestQueryBuilder> addAssignPattern(Synonym arg1, Ref arg2, ExpressionSpec arg3);
        shared_ptr<TestQueryBuilder> addIfPattern(Synonym arg1, Ref arg2);
        shared_ptr<TestQueryBuilder> addWhilePattern(Synonym arg1, Ref arg2);

        shared_ptr<Query> build();


    };

} // TestQE

#endif //SPA_TESTQUERYBUILDER_H
