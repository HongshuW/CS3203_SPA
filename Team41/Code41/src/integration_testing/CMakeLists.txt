file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs}
        ../unit_testing/src/design_extractor/DEUtils.cpp
        ../unit_testing/src/design_extractor/DEUtils.h
        ../unit_testing/src/query_evaluator/QETestUtils.cpp
        ../unit_testing/src/query_evaluator/QETestUtils.h
        ../unit_testing/src/design_extractor/Dummies.cpp
        ../unit_testing/src/design_extractor/Dummies.h
        ../unit_testing/src/query_evaluator/Dummies/TestQueryBuilder.cpp
        ../unit_testing/src/query_evaluator/Dummies/TestQueryBuilder.h
        src/de-pkb-qe/test1.cpp src/qps/TestQueryParserIntegration.cpp src/sp/TestFollowsIntegration.cpp src/sp/TestModifiesIntegration.cpp src/sp/TestParentIntegration.cpp src/sp/TestUsesIntegration.cpp src/sp/TestCallsIntegration.cpp src/sp/TestConditionalPatternIntegration.cpp)


target_link_libraries(integration_testing spa)
