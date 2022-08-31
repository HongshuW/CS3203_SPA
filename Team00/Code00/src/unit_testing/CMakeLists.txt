file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(unit_testing ${srcs} src/query_builder/TestQueryTokenizer.cpp src/query_builder/TestQueryParser.cpp src/query_evaluator/TestQueryEvaluator.cpp src/test_util/QueryBuilder.cpp src/test_util/QueryBuilder.h src/test_util/TypicalQueries.cpp src/test_util/TypicalQueries.h)

target_link_libraries(unit_testing spa)

