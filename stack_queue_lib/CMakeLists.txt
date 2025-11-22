set(target ${STACK_QUEUE_LIBRARY})

add_library(${target} STATIC TStack.h TQueue.h library.cpp)

if(MSVC)
    target_compile_options(${target} PRIVATE "$<$<CONFIG:Debug>:/MDd>" "$<$<CONFIG:Release>:/MD>")
endif()

set_target_properties(${target} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${target} ${LIBRARY_DEPS})