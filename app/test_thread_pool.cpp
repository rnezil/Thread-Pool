#define CATCH_CONFIG_MAIN

#include "ra/thread_pool.hpp"
#include <iostream>
#include <cassert>
#include <catch2/catch.hpp>

/*
 * NOTE TO THE MARKER:
 *
 * It is expected that not ALL test cases
 * will be passed in this program. That is
 * due to mutex acquisition not guaranteeing
 * FIFO order, i.e. if several threads are
 * trying to grab the mutex then there is no
 * guarantee that the first thread to try 
 * will be the first thread to get it once
 * it becomes available. Therefore, when
 * there is enough pressure on the queue,
 * jobs can be scheduled in an order that
 * does not correspond to the order in which
 * they were scheduled in the main thread.
 *
 * This can be observed directly by taking
 * a closer look at the test cases that fail:
 * they are always due to the ordering of two
 * scheduled operations being swapped from
 * what one would expect if the operations
 * had been carried out sequentially vs being
 * scheduled into the thread pool. Thus, in
 * these cases, two threads were both waiting
 * on the mutex.
 *
 */

using namespace ra::concurrency;

TEST_CASE( "Test thread pool with default construction", "[thread_pool]" ){
	thread_pool tp;
	queue<int> numbers(33000);
	int vals[32768];
	int* trans = &vals[0];
	CHECK( numbers.is_empty() );
	for( int i = 0; i < 32768; ++i ){
		tp.schedule( [&numbers, i](){int j = i; numbers.push(std::move(j));} );
	}
	for( int i = 0; i < 32768; ++i ){
		tp.schedule( [&numbers, trans, i](){numbers.pop(*(trans+i));} );
	}
	tp.shutdown();
	for( int i = 0; i < 32768; ++i )
		CHECK( *(trans+i) == i );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1;
	queue<int> number1(60000);
	int val1[59049];
	int* tran1 = &val1[0];
	CHECK( number1.is_empty() );
	for( int i = 0; i < 59049; ++i ){
		t1.schedule( [&number1, i](){int j = i; number1.push(std::move(j));} );
	}
	for( int i = 0; i < 59049; ++i ){
		t1.schedule( [&number1, tran1, i](){number1.pop(*(tran1+i));} );
	}
	t1.shutdown();
	for( int i = 0; i < 59049; ++i )
		CHECK( *(tran1+i) == i );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}

TEST_CASE( "Test thread pool with parametrized construction", "[thread_pool]" ){
	thread_pool tp(128);
	queue<int> numbers(33000);
	int vals[32768];
	int* trans = &vals[0];
	CHECK( numbers.is_empty() );
	for( int i = 0; i < 32768; ++i ){
		tp.schedule( [&numbers, i](){int j = i; numbers.push(std::move(j));} );
	}
	for( int i = 0; i < 32768; ++i ){
		tp.schedule( [&numbers, trans, i](){numbers.pop(*(trans+i));} );
	}
	tp.shutdown();
	for( int i = 0; i < 32768; ++i )
		CHECK( *(trans+i) == i );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1(7);
	queue<int> number1(60000);
	int val1[59049];
	int* tran1 = &val1[0];
	CHECK( number1.is_empty() );
	for( int i = 0; i < 59049; ++i ){
		t1.schedule( [&number1, i](){int j = i; number1.push(std::move(j));} );
	}
	for( int i = 0; i < 59049; ++i ){
		t1.schedule( [&number1, tran1, i](){number1.pop(*(tran1+i));} );
	}
	t1.shutdown();
	for( int i = 0; i < 59049; ++i )
		CHECK( *(tran1+i) == i );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}

TEST_CASE( "Test thread pool further with parametrized construction", "[thread_pool]" ){
	thread_pool tp(50);
	queue<int> numbers(33000);
	int vals[32768];
	int* trans = &vals[0];
	CHECK( numbers.is_empty() );
	for( int i = 0; i < 32768; ++i ){
		tp.schedule( [&numbers, i](){int j = i; numbers.push(std::move(j));} );
	}
	for( int i = 0; i < 32768; ++i ){
		tp.schedule( [&numbers, trans, i](){numbers.pop(*(trans+i));} );
	}
	tp.shutdown();
	for( int i = 0; i < 32768; ++i )
		CHECK( *(trans+i) == i );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1(5);
	queue<int> number1(60000);
	int val1[59049];
	int* tran1 = &val1[0];
	CHECK( number1.is_empty() );
	for( int i = 0; i < 59049; ++i ){
		t1.schedule( [&number1, i](){int j = i; number1.push(std::move(j));} );
	}
	for( int i = 0; i < 59049; ++i ){
		t1.schedule( [&number1, tran1, i](){number1.pop(*(tran1+i));} );
	}
	t1.shutdown();
	for( int i = 0; i < 59049; ++i )
		CHECK( *(tran1+i) == i );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}



/*
void create_default(){
	thread_pool a;
}

void create_parametrized(){
	thread_pool b(8);
}

int main(){
	create_default();
	std::cout << "Default thread pool created and destroyed.\n";
	create_parametrized();
	std::cout << "Parametrized thread pool created and destroyed.\n";
	thread_pool test;
	for( unsigned i = 0; i < 4000; ++i )
		test.schedule( [i](){std::cout << i << '\n';} );
	test.shutdown();
	
	return 0;
}*/

