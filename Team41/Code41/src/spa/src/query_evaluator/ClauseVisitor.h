//
// Created by Xingchen Lin on 28/9/22.
//

#ifndef SPA_CLAUSEVISITOR_H
#define SPA_CLAUSEVISITOR_H

#include "pkb/table/Table.h"
#include "DataPreprocessor.h"
#include <iostream>
#include <variant>
#include "IVisitor.h"

template<class... Ts>
struct overload : Ts ... {
    using Ts::operator()...;
};

template<class... Ts> overload(Ts...) -> overload<Ts...>;
namespace QB {
    class PatternClause;

    class WithClause;

    class SuchThatClause;

    class AssignPatternClause;

    class IfPatternClause;

    class WhilePatternClause;

    class SelectClause;
}


namespace QE {

    typedef shared_ptr<vector<Declaration>> Declarations;
    enum ClauseReturnType {
        BOOL, TABLE
    };
    using Value = variant<string, int, vector<int>, vector<string>>;

    class ClauseVisitor {
        shared_ptr<DataPreprocessor> dataPreprocessor;
        shared_ptr<DataRetriever> dataRetriever;

        void getWithValues(vector<WithRef> withRefs, shared_ptr<vector<Value>> values);

        DesignEntity getDesignEntity(Synonym);
        std::vector<std::string> intersection(std::vector<std::string> v1,
                                              std::vector<std::string> v2);
        std::vector<int> intersection(std::vector<int> v1,
                                              std::vector<int> v2);

        Declarations declarations;
        ClauseReturnType returnType;

    public:
        explicit ClauseVisitor(shared_ptr<DataPreprocessor> dataPreprocessor,
                               shared_ptr<DataRetriever> dataRetriever,
                               Declarations declarations);
        ClauseReturnType setReturnTable();
        ClauseReturnType setReturnBool();


//        Table operator()(shared_ptr<PatternClause> patternClause);
        Table operator()(shared_ptr<AssignPatternClause> assignPatternClause);

        Table operator()(shared_ptr<IfPatternClause> ifPatternClause);

        Table operator()(shared_ptr<WhilePatternClause> whilePatternClause);

        Table operator()(shared_ptr<SuchThatClause> suchThatClause);

        Table operator()(shared_ptr<WithClause> withClause);

        Table operator()(shared_ptr<SelectClause> selectClause);
    };

} // QE

#endif //SPA_CLAUSEVISITOR_H
