set(target ${LAB_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})
target_link_libraries(${target} gtest ${STACK_QUEUE_LIBRARY} ${ARITHM_EXPR_LIBRARY})
# target_include_directories(${target} ${STACK_QUEUE_INCLUDE} ${ARITHM_EXPR_INCLUDE})

#include(GoogleTest)
#add_test(NAME ${TARGET} COMMAND ${TARGET})