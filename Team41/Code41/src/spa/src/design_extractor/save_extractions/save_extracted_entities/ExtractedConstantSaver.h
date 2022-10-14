//
// Created by Aaron on 14/10/22.
//

#ifndef SPA_EXTRACTEDCONSTANTSAVER_H
#define SPA_EXTRACTEDCONSTANTSAVER_H

#include "../save_to_pkb/PKBSaverInterface.h"

class ExtractedConstantSaver : public PKBSaverInterface {
public:
    ExtractedConstantSaver(shared_ptr<ProgramNode> programNode,
                           shared_ptr<DataModifier> dataModifier)
    : PKBSaverInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<AST::ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;

};


#endif //SPA_EXTRACTEDCONSTANTSAVER_H
