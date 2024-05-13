file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers})
# Exclude Main.cpp (used for standalone execution separate from autotester) from the spa library sources
list(REMOVE_ITEM srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(main "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")
target_link_libraries(main spa)