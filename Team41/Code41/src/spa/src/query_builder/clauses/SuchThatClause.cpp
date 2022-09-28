//
// Created by Xingchen Lin on 31/8/22.
//

#include "SuchThatClause.h"

#include <utility>


using namespace QB;

SuchThatClause::SuchThatClause(RelationType relationType,
                               Ref arg1,
                               Ref arg2,
                               shared_ptr<vector<Declaration>> declarations) :
        relationType(relationType),
        arg1(std::move(std::move(arg1))),
        arg2(std::move(std::move(arg2))),
        declarations(std::move(declarations)) {
    if (relationType == RelationType::MODIFIES || relationType == RelationType::USES) {
        updateSpecificModifiesUsesClause();
    }
}

void SuchThatClause::updateSpecificModifiesUsesClause() {
    if (getRefType(arg1) == RefType::INTEGER) {
        // Since arg1 is an integer, it must be a stmtRef
        relationType = relationType == RelationType::USES ? RelationType::USES_S : RelationType::MODIFIES_S;
    } else if (getRefType(arg1) == RefType::IDENT) {
        // Since arg1 is an ident, it must be a entRef
        relationType = relationType == RelationType::USES ? RelationType::USES_P : RelationType::MODIFIES_P;
    } else if (auto synonym = get_if<Synonym>(&arg1)){
        // Since arg1 is a synonym, we need to check if the synonym is declared as procedure
        if (auto declaration = Declaration::findDeclaration(*synonym, declarations)) {
            if (declaration->getDesignEntity() == DesignEntity::PROCEDURE) {
                relationType = relationType == RelationType::USES ? RelationType::USES_P : RelationType::MODIFIES_P;
            } else {
                relationType = relationType == RelationType::USES ? RelationType::USES_S : RelationType::MODIFIES_S;
            }
        }
    }
}

bool SuchThatClause::operator==(const SuchThatClause& clause) const {
    auto suchThatClause = dynamic_cast<const SuchThatClause*>(&clause);
    return suchThatClause != nullptr && arg1 == suchThatClause->arg1 && arg2 == suchThatClause->arg2;
}

ostream& SuchThatClause::print(ostream& os) const {
    // Print the derived class specific information.
    os << getStrFromRelationType(relationType) << "Clause;";
    return os;
}

