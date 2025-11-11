set(target ${STACK_QUEUE_LIBRARY})

add_library(${target} STATIC TStack.h TQueue.h library.cpp)
set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${target} ${LIBRARY_DEPS})
