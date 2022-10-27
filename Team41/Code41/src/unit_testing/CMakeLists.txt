file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(unit_testing ${srcs}
    src/query_builder/TestQueryTokenizer.cpp
    src/query_builder/TestQueryParser.cpp
    src/query_builder/TestQueryValidator.cpp
    src/parser/TestTokenizer.cpp
    src/parser/TestParser.cpp
    src/pkb/TestTable.cpp
    src/pkb/TestDataRetriever.cpp
    src/pkb/TestDataModifier.cpp
    src/query_evaluator/TestDataPreprocessor.cpp
    src/query_evaluator/TestTableCombiner.cpp
    src/query_evaluator/TableComparator.cpp
    src/query_evaluator/TableComparator.h
    src/query_evaluator/Dummies/TableBuilder.cpp
    src/query_evaluator/Dummies/TableBuilder.h
    src/query_evaluator/DummyDataRetrievers/DummyDataRetriever.cpp
    src/query_evaluator/DummyDataRetrievers/DummyDataRetriever.h
    src/AST/utils/TestASTUtils.cpp
    src/parser/TestParser.cpp
    src/design_extractor/TestEntityExtraction.cpp
    src/design_extractor/Dummies.cpp
    src/design_extractor/Dummies.h
    src/design_extractor/TestFollowsExtraction.cpp
    src/design_extractor/TestFollowsStarExtraction.cpp
    src/design_extractor/TestParentExtraction.cpp
    src/parser/TestExprNodeParser.cpp
    src/design_extractor/TestUsesSExtration.cpp
    src/design_extractor/DEUtils.cpp
    src/design_extractor/DEUtils.h
    src/design_extractor/TestModifiesSExtraction.cpp
    src/parser/TestExprNodeParser.cpp
    src/parser/TestCondExprParser.cpp src/utils/TestUtils.cpp
    src/design_extractor/TestUsesSExtration.cpp
    src/design_extractor/DEUtils.cpp
    src/query_evaluator/DummyDataRetrievers/DummyDataRetriever2.cpp
    src/query_evaluator/DummyDataRetrievers/DummyDataRetriever2.h
    src/design_extractor/DEUtils.h
    src/parser/TestExprNodeParser.cpp
    src/parser/TestCondExprParser.cpp
    src/utils/TestUtils.cpp
    src/design_extractor/TestPatternExtraction.cpp
    src/query_evaluator/QETestUtils.cpp
    src/query_evaluator/QETestUtils.h
    src/parser/TestCondExprParser.cpp
    src/utils/TestUtils.cpp
    src/design_extractor/TestPatternExtraction.cpp
    src/query_evaluator/TestQueryOptimizer.cpp
    src/query_builder/TestExprStringTokenizer.cpp
    src/pkb/TestPKBStorage.cpp
    src/query_evaluator/TestQueryEvaluator.cpp
    src/design_extractor/TestCallsExtraction.cpp
    src/design_extractor/TestCallsStarExtraction.cpp
    src/design_extractor/TestModifiesPExtraction.cpp
    src/design_extractor/TestUsesPExtraction.cpp
    src/CFG/TestCFG.cpp
    src/parser/TestASTValidator.cpp
    src/parser/InvalidProcedureDummies.cpp
    src/parser/InvalidProcedureDummies.h
    src/query_evaluator/Dummies/TestQueryBuilder.cpp 
    src/query_evaluator/Dummies/TestQueryBuilder.h
    src/design_extractor/TestAffectsExtraction.cpp
    src/design_extractor/TestAffectsStarExtraction.cpp
    src/design_extractor/TestNextExtraction.cpp
    src/design_extractor/TestNextStarExtraction.cpp
    src/pkb/TestCachable.cpp
    src/design_extractor/TestConditionalPatternExtractor.cpp
        src/design_extractor/TestCallsRelationExtractor.cpp
        src/design_extractor/TestCallsTRelationExtractor.cpp
    src/design_extractor/TestConditionalPatternExtractor.cpp
        src/design_extractor/TestNextRelationExtractor.cpp src/design_extractor/TestNextTRelationExtractor.cpp)


target_link_libraries(unit_testing spa)
