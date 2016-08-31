cmake_minimum_required (VERSION 2.6)
project (Enumerable)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
 
#message(file(GLOB Incudes "${PROJECT_SOURCE_DIR}/include/*.h"))

#message(${Includes})
message("CMAKE version:")
execute_process(COMMAND cmake --version)

include_directories ("${PROJECT_SOURCE_DIR}/include")
add_executable(EnumerableTest test/main.cpp ${Includes})

include(CTest)
add_subdirectory(test)

