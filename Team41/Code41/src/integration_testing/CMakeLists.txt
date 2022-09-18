file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_executable(integration_testing ${srcs}
        ../unit_testing/src/design_extractor/DEUtils.cpp
        ../unit_testing/src/design_extractor/DEUtils.h
        src/sp/TestSPIntegration.cpp)


target_link_libraries(integration_testing spa)
