add_library(libtree tree.cpp)

target_link_libraries(libtree PRIVATE liblogs)

target_include_directories(libtree PUBLIC
                    ${CMAKE_CURRENT_LIST_DIR})