add_library(libgraph graph.cpp)

target_link_libraries(libgraph PRIVATE liblogs)

target_include_directories(libgraph PUBLIC
                        ${CMAKE_CURRENT_LIST_DIR})