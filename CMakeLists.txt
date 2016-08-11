cmake_minimum_required (VERSION 2.6)
project (Enumerable)

#message(file(GLOB Incudes "${PROJECT_SOURCE_DIR}/include/*.h"))

#message(${Includes})

include_directories ("${PROJECT_SOURCE_DIR}/include")
add_executable(EnumerableTest test/main.cpp ${Includes})

include(CTest)
add_subdirectory(test)

