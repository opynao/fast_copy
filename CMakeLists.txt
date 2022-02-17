cmake_minimum_required(VERSION 3.20)

set (CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})
# set project name
project(fast_copy VERSION 1.0 DESCRIPTION "Crossplatform fast copy utility" LANGUAGES CXX)

#set(CMAKE_CXX_COMPILER clang++ )

# set generator
if(WIN32)
	set(CMAKE_GENERATOR "Visual Studio 17 2022")
	message(STATUS "Generator is set to ${CMAKE_GENERATOR}")
endif()

# Disable in-source builds to prevent source tree corruption.
if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.")
endif()

# set c++ standard & compiler
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
MESSAGE(STATUS "${CMAKE_CXX_COMPILER}")

# set compiler options
if( UNIX )
    if(Debug)
        add_definitions(" -O0 -g -Wall -Wno-reorder")
    else()
        add_definitions(" -O3 -Wall -Wno-reorder")
        #-Werror -Wextra -Wconversion -Wsign-conversion -g3 -Og
    endif(Debug)

    # add_definitions(" -pedantic -pedantic-errors -W ")

elseif( WIN32 )
    add_definitions( -D_SCL_SECURE_NO_WARNINGS )
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )

endif( UNIX )

# set options
option(ENABLE_TESTS "Set to ON to enable building of tests" ON)
if (ENABLE_TESTS)
    message(STATUS "Building of tests is enabled")
	add_subdirectory(googletest)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(spdlog)
add_subdirectory(fast_copy)