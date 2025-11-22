cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME LAB-3)
project(${PROJECT_NAME})

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

set(STACK_QUEUE_LIB_NAME stack_queue_lib)
set(STACK_QUEUE_LIBRARY "${PROJECT_NAME}_${STACK_QUEUE_LIB_NAME}")
set(STACK_QUEUE_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${STACK_QUEUE_LIB_NAME}")

set(ARITHM_EXPR_LIB_NAME arithm_expr_lib)
set(ARITHM_EXPR_LIBRARY "${PROJECT_NAME}_${ARITHM_EXPR_LIB_NAME}")
set(ARITHM_EXPR_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/${ARITHM_EXPR_LIB_NAME}/include")

set(LAB_TESTS "test_${PROJECT_NAME}")

include_directories(${STACK_QUEUE_INCLUDE})
include_directories(${ARITHM_EXPR_INCLUDE})

add_subdirectory(${STACK_QUEUE_LIB_NAME})
add_subdirectory(${ARITHM_EXPR_LIB_NAME})

set(BENCHMARK_ENABLE_TESTING OFF)
set(HAVE_STD_REGEX 0)
set(HAVE_STEADY_CLOCK 0)
set(HAVE_POSIX_REGEX 0)

add_subdirectory("googletest/" EXCLUDE_FROM_ALL)

enable_testing()

include(GoogleTest)
add_subdirectory(tests)