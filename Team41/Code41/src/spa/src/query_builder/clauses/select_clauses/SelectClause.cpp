//
// Created by Xingchen Lin on 29/8/22.
//

#include "SelectClause.h"

#include <utility>

using namespace QB;

SelectClause::SelectClause(ReturnType returnType) : returnType(returnType){}

SelectClause::SelectClause(ReturnType returnType, shared_ptr<vector<Elem>> returnResults) :
        returnType(returnType), returnResults(std::move(returnResults)) {}

bool SelectClause::isBoolean() const {
    return returnType == ReturnType::BOOLEAN;
}

bool SelectClause::isTuple() const {
    return returnType == ReturnType::TUPLE;
}

bool SelectClause::operator==(const SelectClause& clause) const {
    auto selectClause = dynamic_cast<const SelectClause*>(&clause);
    bool isEqual = selectClause != nullptr && returnType == selectClause->returnType;
    if (returnResults) {
        isEqual = isEqual && equal(begin(*returnResults), end(*returnResults),
                                   begin(*selectClause->returnResults),
                                   end(*selectClause->returnResults),
                                   [](const Elem& l, const Elem& o)
                                   {return l == o;});
    }
    return isEqual;
}

ostream& SelectClause::print(ostream& os) const {
    // Print the derived class specific information.
    os << "Select Clause, return results include: ";
    for (const auto& result : *(returnResults)) {
        //TODO: implement this later
    }
    return os;
}

Table SelectClause::accept(shared_ptr<IVisitor> visitor) {
    return visitor->visit(shared_from_this());
}
