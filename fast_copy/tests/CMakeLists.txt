# sources

file(GLOB_RECURSE TEST_SRC_LIST ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(LIB_LIST_TESTS
    spdlog::spdlog
    GTest::gtest
    GTest::gmock
    ${PROJECT_NAME}_lib
)
include_directories( ${CMAKE_SOURCE_DIR}/include )

add_executable(${PROJECT_NAME}_tests ${TEST_SRC_LIST})
target_link_libraries(${PROJECT_NAME}_tests ${LIB_LIST_TESTS})
add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)