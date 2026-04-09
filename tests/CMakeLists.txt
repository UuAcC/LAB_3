set(target ${LAB_TESTS})

file(GLOB hdrs "*.h*")
file(GLOB srcs "*.cpp")

add_executable(${target} ${srcs} ${hdrs})

if(MSVC)
    target_compile_options(${target} PRIVATE "$<$<CONFIG:Debug>:/MDd>" "$<$<CONFIG:Release>:/MD>")
endif()

target_link_libraries(${target} gtest gtest_main ${DATA_STRUCTURES_LIBRARY} ${PMME_LIBRARY})

gtest_discover_tests(${target})