file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_executable(unit_testing ${srcs} src/query_builder/TestQueryTokenizer.cpp src/query_builder/TestQueryParser.cpp src/query_builder/TestQueryValidator.cpp src/parser/TestTokenizer.cpp src/parser/TestParser.cpp src/pkb/TestTable.cpp src/pkb/TestEntityManager.cpp src/pkb/TestDataRetriever.cpp src/query_evaluator/TestQueryEvaluator.cpp src/query_evaluator/TestQueryResultFormatter.cpp)

target_link_libraries(unit_testing spa)

