//
// Created by Aaron on 14/10/22.
//

#ifndef SPA_EXTRACTEDPROCEDURESSAVER_H
#define SPA_EXTRACTEDPROCEDURESSAVER_H

#include "../save_to_pkb/PKBSaverInterface.h"

class ExtractedProceduresSaver : public PKBSaverInterface {
public:
    ExtractedProceduresSaver(shared_ptr<ProgramNode> programNode,
                             shared_ptr<DataModifier> dataModifier)
    : PKBSaverInterface(std::move(programNode), std::move(dataModifier)) {}

    void save(shared_ptr<ProgramNode> programNode, shared_ptr<DataModifier> dataModifier) override;
};


#endif //SPA_EXTRACTEDPROCEDURESSAVER_H
