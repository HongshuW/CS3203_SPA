////
//// Created by Nafour on 11/9/22.
////
//
//#include "DummyQueries.h"
//
//namespace TestQE {
//    SuchThatClause DummyQueries::getDummySuchThat(int idx) {
//        //assign a1, a2, get table such that follows(a1, a2)
//        shared_ptr<vector<Declaration>> declarations = make_shared<vector<Declaration>>();
//        Synonym syn1 = Synonym("a1");
//        Synonym syn2 = Synonym("a2");
//        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1)});
//        declarations->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2)});
//        SuchThatClause suchThatClause = SuchThatClause(QB::RelationType::FOLLOWS, syn1, syn2, declarations);
//
//
//        //while w, get table such that followsT(1, w)
//        shared_ptr<vector<Declaration>> declarations_2 = make_shared<vector<Declaration>>();
//        Synonym syn2_2 = Synonym("w");
//        declarations_2->push_back({Declaration(QB::DesignEntity::WHILE, syn2_2)});
//        SuchThatClause suchThat_2 = SuchThatClause(QB::RelationType::FOLLOWS_T, 1, syn2_2, declarations_2);
//
//        //assign a, get table such that Parent(_, a)
//        shared_ptr<vector<Declaration>> declarations_3 = make_shared<vector<Declaration>>();
//        Synonym syn2_3 = Synonym("a");
//        declarations_3->push_back({Declaration(QB::DesignEntity::ASSIGN, syn2_3)});
//        SuchThatClause suchThat_3 = SuchThatClause(QB::RelationType::PARENT, Underscore(), syn2_3, declarations_3);
//
//        // assign a, get table such that Uses(a, 'y')
//        shared_ptr<vector<Declaration>> declarations_4 = make_shared<vector<Declaration>>();
//        Synonym syn1_4 = Synonym("a");
//        declarations_4->push_back({Declaration(QB::DesignEntity::ASSIGN, syn1_4)});
//        SuchThatClause suchThatClause_4 = SuchThatClause(QB::RelationType::USES_S, syn1_4, Ident("y"), declarations_4);
//
//
//        vector<SuchThatClause> dummies = {suchThatClause , suchThat_2, suchThat_3, suchThatClause_4};
//        return dummies[idx];
//    }
//
//} // TestQE