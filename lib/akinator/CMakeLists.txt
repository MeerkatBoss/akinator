add_library(libakinator akinator.h)

target_link_libraries(libakinator PRIVATE liblogs)
target_link_libraries(libakinator PUBLIC libtree)

target_include_directories(libakinator PUBLIC
                        ${CMAKE_CURRENT_LIST_DIR})