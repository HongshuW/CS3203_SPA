file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(unit_testing ${srcs} src/query_builder/TestQueryTokenizer.cpp src/query_builder/TestQueryParser.cpp src/query_builder/TestQueryValidator.cpp src/parser/TestTokenizer.cpp src/parser/TestParser.cpp src/pkb/TestTable.cpp src/pkb/TestEntityManager.cpp src/pkb/TestDataRetriever.cpp src/pkb/TestDataModifier.cpp src/query_evaluator/TestQueryEvaluator.cpp src/query_evaluator/TestQueryResultFormatter.cpp src/design_extractor/TestVariableExtractor.cpp src/query_evaluator/Dummies/DummyQueryBuilder.cpp src/query_evaluator/Dummies/DummyQueryBuilder.h src/query_evaluator/TestDataPreprocessor.cpp src/query_evaluator/TestTableCombiner.cpp src/query_evaluator/TableComparator.cpp src/query_evaluator/TableComparator.h src/query_evaluator/Dummies/TableBuilder.cpp src/query_evaluator/Dummies/TableBuilder.h src/query_evaluator/DummyDataRetriever.cpp src/query_evaluator/DummyDataRetriever.h src/AST/utils/TestASTUtils.cpp)

target_link_libraries(unit_testing spa)
