# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.20)

# set project name
project(fast_copy VERSION 1.0 DESCRIPTION "Crossplatform fast copy utility" LANGUAGES CXX)

# get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
# string(REPLACE " " "_" ProjectId ${ProjectId})
# project(${ProjectId})

# options
option(ENABLE_TESTS "Set to ON to enable building of tests" ON)
#if (ENABLE_TESTS)
#    message(STATUS "Building of tests is enabled")
#    enable_testing()
#    add_subdirectory(tests)
#endif()

# c++ standard & compiler options
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_COMPILER "/usr/bin/g++")

if(MSVC)
    MESSAGE("Compiler: MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++latest /W4")
    #set(CMAKE_CXX_FLAGS_RELEASE "{CMAKE_CXX_FLAGS_RELEASE} /O2")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    if (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC") # clang-cl
        MESSAGE("Compiler: clang-cl")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
        #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    elseif (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "GNU") # clang native
        MESSAGE("Compiler: clang native")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
        #set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    endif()    
elseif(CMAKE_COMPILER_IS_GNUCXX)
    MESSAGE("Compiler: GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
    #-Werror -Wextra
endif()

# # libs
# Include sub-projects.
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/include
                     ${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include
)

file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_library(${PROJECT_NAME}_lib ${SRC_FILES})

set(LIB_LIST
    ${PROJECT_NAME}_lib
    spdlog::spdlog
)

# building
add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} ${LIB_LIST})
target_include_directories(${PROJECT_NAME} PUBLIC include)
add_subdirectory(spdlog)