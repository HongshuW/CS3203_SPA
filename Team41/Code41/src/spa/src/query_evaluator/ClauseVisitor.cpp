//
// Created by Xingchen Lin on 28/9/22.
//

#include "ClauseVisitor.h"


QE::ClauseVisitor::ClauseVisitor(
        shared_ptr<DataPreprocessor> dataPreprocessor,QE::Declarations declarations)
        : dataPreprocessor(dataPreprocessor), declarations(declarations) {

}

Table QE::ClauseVisitor::operator()(shared_ptr<PatternClause> patternClause) {
    return dataPreprocessor->getTableByPattern(patternClause);
}

Table QE::ClauseVisitor::operator()(shared_ptr<SuchThatClause> suchThatClause) {
    return dataPreprocessor->getTableByRelation(*suchThatClause);
}

Table QE::ClauseVisitor::operator()(shared_ptr<WithClause> withClause) {
    return dataPreprocessor->getTableByWith(withClause, declarations);
}
