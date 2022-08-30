//
// Created by Xingchen Lin on 28/8/22.
//

#ifndef SPA_CLAUSE_H
#define SPA_CLAUSE_H

#include <iostream>

namespace QB {
    class Clause {
    protected:
        virtual std::ostream & print(std::ostream& os) const {
            return os;
        }
    public:
        virtual void accept() = 0;
        friend std::ostream& operator<<(std::ostream& os, Clause const& clause) {
            return clause.print(os);
        }

        virtual ~Clause() {}
    };
}

#endif //SPA_CLAUSE_H
