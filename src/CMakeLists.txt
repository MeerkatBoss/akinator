add_executable(akinator main.cpp io_utils.cpp)

target_link_libraries(akinator liblogs libtree libakinator)

target_include_directories(akinator PRIVATE
                        ${CMAKE_CURRENT_LIST_DIR})