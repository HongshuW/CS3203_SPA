file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(spa ${srcs} ${headers} src/query_builder/QueryBuilder.h src/query_builder/QueryBuilder.cpp src/query_builder/QueryTokenizer.h src/query_builder/QueryTokenizer.cpp src/query_builder/Exceptions.h src/query_builder/commons/Query.h src/query_builder/commons/Query.cpp src/query_builder/commons/DesignEntity.h src/query_builder/commons/DesignEntity.cpp src/query_builder/commons/Synonym.h src/query_builder/commons/Synonym.cpp src/query_builder/QueryParser.h src/query_builder/QueryParser.cpp src/query_builder/commons/Declaration.h src/query_builder/clauses/SelectClause.h src/query_builder/clauses/SelectClause.cpp src/query_builder/clauses/SuchThatClause.h src/query_builder/commons/Ref.h src/query_builder/commons/Underscore.h src/query_builder/commons/RelationType.h src/query_builder/commons/RelationType.cpp src/query_builder/clauses/SuchThatClause.cpp src/query_builder/commons/Ref.cpp src/pkb/DataModifier.cpp src/pkb/DataModifier.h src/pkb/DataRetriever.cpp src/pkb/DataRetriever.h src/pkb/Table.cpp src/pkb/Table.h src/pkb/EntityManager.cpp src/pkb/EntityManager.h src/query_evaluator/QueryEvaluator.cpp src/query_evaluator/QueryEvaluator.h src/query_evaluator/QueryResult.cpp src/query_evaluator/QueryResult.h src/query_evaluator/QueryResultFormatter.cpp src/query_evaluator/QueryResultFormatter.h)

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


