file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")
add_library(spa ${srcs} ${headers}
        src/QPS/Parser/ClauseParser.cpp
        src/QPS/Parser/ClauseParser.h
        src/QPS/QueryError.h
        src/QPS/Parser/ClauseValidator.cpp
        src/QPS/Parser/ClauseValidator.h
        src/QPS/Parser/DeclarationsValidator.cpp
        src/QPS/Parser/DeclarationsValidator.h
        src/QPS/Parser/DeclarationsParser.cpp
        src/QPS/Parser/DeclarationsParser.h
)
# this makes the headers accessible for other projects which uses spa lib
target_include_directories(spa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)


