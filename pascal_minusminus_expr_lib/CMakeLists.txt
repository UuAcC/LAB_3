set(target ${PMME_LIBRARY})

file(GLOB hdrs "include/*.h*" "${PMME_INCLUDE}/*.h*")
file(GLOB srcs "source/*.cpp*")

add_library(${target} STATIC ${srcs} ${hdrs})

if(MSVC)
    target_compile_options(${target} PRIVATE "$<$<CONFIG:Debug>:/MDd>" "$<$<CONFIG:Release>:/MD>")
endif()

target_link_libraries(${target} ${LIBRARY_DEPS})
target_link_libraries(${target} ${DATA_STRUCTURES_LIBRARY})