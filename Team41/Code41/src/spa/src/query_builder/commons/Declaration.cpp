//
// Created by Xingchen Lin on 31/8/22.
//

#include "Declaration.h"

using namespace std;
using namespace QB;

Declaration::Declaration(DesignEntity designEntity, Synonym synonym)
: designEntity(designEntity), synonym(synonym){};

optional<Declaration> Declaration::findDeclaration(Synonym& synonymObj, shared_ptr<vector<Declaration>> declarations) {
    for (auto declaration : *declarations) {
        if (declaration.getSynonym().synonym.compare(synonymObj.synonym) == 0) {
            return make_optional(declaration);
        }
    }
    return nullopt;
}

bool Declaration::operator==(const Declaration& a2) const {
    return designEntity == a2.designEntity && synonym == a2.synonym;
}