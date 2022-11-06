//
// Created by Nafour on 30/9/22.
//

#include "AbstractClauseVisitor.h"

#include <utility>

AbstractClauseVisitor::AbstractClauseVisitor(
    shared_ptr<QE::DataPreprocessor> dataPreprocessor)
    : dataPreprocessor(std::move(dataPreprocessor)) {}
