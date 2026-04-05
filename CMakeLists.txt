cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME LAB-3)
project(${PROJECT_NAME})

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

set(DATA_STRUCTURES_LIB_NAME data_structures_lib)
set(DATA_STRUCTURES_LIBRARY "${PROJECT_NAME}_${DATA_STRUCTURES_LIB_NAME}")
set(DATA_STRUCTURES_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${DATA_STRUCTURES_LIB_NAME}")

set(PMME_LIB_NAME pascal_minusminus_expr_lib)
set(PMME_LIBRARY "${PROJECT_NAME}_${PMME_LIB_NAME}")
set(PMME_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${PMME_LIB_NAME}/include")

set(LAB_TESTS "test_${PROJECT_NAME}")

include_directories(${DATA_STRUCTURES_INCLUDE})
include_directories(${PMME_INCLUDE})

add_subdirectory(${DATA_STRUCTURES_LIB_NAME})
add_subdirectory(${PMME_LIB_NAME})

set(BENCHMARK_ENABLE_TESTING OFF)
set(HAVE_STD_REGEX 0)
set(HAVE_STEADY_CLOCK 0)
set(HAVE_POSIX_REGEX 0)

add_subdirectory("googletest/" EXCLUDE_FROM_ALL)

enable_testing()

include(GoogleTest)
add_subdirectory(tests)