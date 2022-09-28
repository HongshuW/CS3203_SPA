//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_DECLARATION_H
#define SPA_DECLARATION_H

#pragma once

#include <iostream>
#include <regex>
#include <string>
#include <ostream>
#include "query_builder/commons/DesignEntity.h"
#include "query_builder/commons/Synonym.h"

namespace QB {
//! Represents a single declaration in a PQL query (e.g. 'assign a')
    class Declaration {
    public:
        DesignEntity designEntity;
        Synonym synonym;
        DesignEntity getDesignEntity() { return designEntity; }
        Synonym getSynonym() { return synonym; }

        Declaration(DesignEntity designEntity, Synonym synonym);

        bool operator==(const Declaration& a2) const;
        friend std::ostream& operator<<(std::ostream& os, Declaration const& declaration) {
            os << "Declaration, synonym: " << declaration.synonym <<
               " designEntity: " << getDesignEntityString(declaration.designEntity);
            return os;
        };

        static optional<Declaration> findDeclaration(Synonym& synonymObj, shared_ptr<vector<Declaration>> declarations);
    };
}
#endif //SPA_DECLARATION_H
