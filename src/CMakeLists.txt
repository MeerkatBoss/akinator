add_executable(akinator main.cpp)

target_link_libraries(akinator liblogs libtree)

target_include_directories(akinator PRIVATE
                        ${CMAKE_CURRENT_LIST_DIR})