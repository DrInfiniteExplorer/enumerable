
include_directories(../include)

function(add_named_test TEST_NAME TEST_SOURCE)
  get_filename_component(TEST_TARGET ${TEST_SOURCE} NAME_WE)
  add_executable(${TEST_TARGET} ${TEST_SOURCE})

  add_test(${TEST_NAME} ${TEST_TARGET})
endfunction()

function(expect TEST_NAME OUTPUT)
  set_tests_properties (${TEST_NAME}
    PROPERTIES PASS_REGULAR_EXPRESSION ${OUTPUT}
	)	
endfunction()

add_named_test("heap.enumerable" test_heap_enumerable.cpp)
expect("heap.enumerable" "\n0 1 2 3 4 5 6 \n")
expect("heap.enumerable" "\n0.000000 1.000000 2.000000 3.000000 4.000000 5.000000 6.000000 \n")
 
add_named_test("iterator.foreach" test_iterator_foreach.cpp)
expect("iterator.foreach" "^0 1 2 3 4 5 6 \n$")

add_named_test("tocontainer" test_tocontainer.cpp)

add_named_test("foreach.lambda" test_foreach_lambda.cpp)
expect("foreach.lambda" "^N 1 2 3 \n")
expect("foreach.lambda" "\nRR 1 2 3 \n")
expect("foreach.lambda" "\nCR 1 2 3 \n")
expect("foreach.lambda" "\nC 1 2 3 \n")	
expect("foreach.lambda" "\nLN 1 2 3 \n")
expect("foreach.lambda" "\nLRR 1 2 3 \n")
expect("foreach.lambda" "\nLCR 1 2 3 \n")
expect("foreach.lambda" "\nLC 1 2 3 \n")

