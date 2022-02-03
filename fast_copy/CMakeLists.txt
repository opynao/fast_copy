# options
if (ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_SOURCE_DIR}/spdlog/include )

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME}_lib ${SRC_FILES})

set(LIB_LIST
#   ${PROJECT_NAME}_lib
    spdlog::spdlog
)

# building
add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})
#target_include_directories(${PROJECT_NAME} PUBLIC include)