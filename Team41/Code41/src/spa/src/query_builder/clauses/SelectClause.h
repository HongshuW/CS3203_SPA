//
// Created by Xingchen Lin on 29/8/22.
//

#ifndef SPA_SELECTCLAUSE_H
#define SPA_SELECTCLAUSE_H

#pragma once

#include "query_builder/commons/Synonym.h"
#include "query_builder/commons/AttrRef.h"
#include <ostream>

namespace QB {
    enum class ReturnType {
        TUPLE,
        BOOLEAN
    };

    using Elem = variant<Synonym, AttrRef>;

    class SelectClause {
    public:
        //! Can be Synonym, Tuple or BOOLEAN
        ReturnType returnType;
        //! To store results for single synonym and tuple only;
        shared_ptr<vector<Elem>> returnResults;
        //! For BOOLEAN
        SelectClause(ReturnType returnType);
        //! For Tuple
        SelectClause(ReturnType returnType, shared_ptr<vector<Elem>> returnResults);

        bool isBoolean();
        bool isTuple();

        bool operator==(const SelectClause& selectClause1) const {
            return returnType == selectClause1.returnType &&
                equal(begin(*returnResults), end(*returnResults),
                      begin(*selectClause1.returnResults),end(*selectClause1.returnResults),
                      [](const Elem l, const Elem o) {
                    return l == o;
                });
        }

        // For printing
        std::ostream & print(std::ostream & os) const {
            // Print the derived class specific information.
            os << "Select Clause, return results include: ";
            for (const auto& result : *(returnResults)) {
                //TODO: implement this later
            }
            return os;
        }
    };
}

#endif //SPA_SELECTCLAUSE_H
