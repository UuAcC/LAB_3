set(target ${LAB_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})

target_compile_options(${target} PRIVATE "$<$<CXX_COMPILER_ID:MSVC>:/MDd>")

target_link_libraries(${target} gtest gtest_main ${STACK_QUEUE_LIBRARY} ${ARITHM_EXPR_LIBRARY})

gtest_discover_tests(${target})