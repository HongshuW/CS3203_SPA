//
// Created by Nafour on 30/9/22.
//

#ifndef SPA_ABSTRACTCLAUSEVISITOR_H
#define SPA_ABSTRACTCLAUSEVISITOR_H
#include "IVisitor.h"
#include "DataPreprocessor.h"

class AbstractClauseVisitor: public IVisitor {


protected:
    shared_ptr<QE::DataPreprocessor> dataPreprocessor;

public:
     explicit AbstractClauseVisitor(shared_ptr<QE::DataPreprocessor> dataPreprocessor);
    ~AbstractClauseVisitor() override = default;
};


#endif //SPA_ABSTRACTCLAUSEVISITOR_H
