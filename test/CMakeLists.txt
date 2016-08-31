
set(CMAKE_SRC_ROOT https://github.com/google/googletest.git CACHE STRING "Root of gtest repo to use")

set(BUILD_GTEST ON)
if(WIN32)
	set(gtest_disable_pthreads ON)
endif()

set(gtest_force_shared_crt ON)

string(FIND ${CMAKE_SRC_ROOT} "http" pos1)
if(${pos1} GREATER "-1")

	if(NOT IS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gtest)
		message("Will clone from git url")
		execute_process(COMMAND git clone ${CMAKE_SRC_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/gtest --depth=1)
		execute_process(COMMAND echo asd)

		include(CheckCXXSourceCompiles)
		unset(IS_BAD_MINGW CACHE)
		CHECK_CXX_SOURCE_COMPILES(
			"#include <windows.h>
			_RTL_CRITICAL_SECTION cs;
			int main(int argc, char* argv[]){}"
		IS_BAD_MINGW)
		if(IS_BAD_MINGW)
			message("Will patch gtest's bad assumption about mingw")
			execute_process(COMMAND git -C ${CMAKE_CURRENT_BINARY_DIR}/gtest apply ${CMAKE_CURRENT_SOURCE_DIR}/gtest_fix/fix.diff)
		endif()
	endif()
	add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/gtest ${CMAKE_CURRENT_BINARY_DIR}/gtest_build)
	
else()
	add_subdirectory(${CMAKE_SRC_ROOT})
endif()

include_directories(../include)

function(add_named_test TEST_NAME TEST_SOURCE)
  get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_TARGET} ${TEST_SOURCE})
  target_link_libraries (${TEST_TARGET} LINK_PUBLIC gtest gmock)
  add_test(${TEST_NAME} ${TEST_TARGET})
endfunction()

function(add_google_test TEST_SOURCE)
  get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_TARGET} ${TEST_SOURCE})
  target_link_libraries (${TEST_TARGET} LINK_PUBLIC gtest_main gmock)
  add_test(${TEST_TARGET} ${TEST_TARGET})
endfunction()


function(expect TEST_NAME OUTPUT)
  set_tests_properties (${TEST_NAME}
    PROPERTIES PASS_REGULAR_EXPRESSION ${OUTPUT}
	)	
endfunction()

add_google_test(test_sequenceEqual.cpp)

add_named_test("heap.enumerable" test_heap_enumerable.cpp)
expect("heap.enumerable" "\n0 1 2 3 4 5 6 \n")
expect("heap.enumerable" "\n0.000000 1.000000 2.000000 3.000000 4.000000 5.000000 6.000000 \n")
 
add_google_test(test_iterator_foreach.cpp)

add_google_test(test_tocontainer.cpp)

add_google_test(test_foreach_lambda.cpp)
