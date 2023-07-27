#define CATCH_CONFIG_MAIN

#include "ra/thread_pool.hpp"
#include <iostream>
#include <cassert>
#include <set>
#include <catch2/catch.hpp>

using namespace ra::concurrency;

TEST_CASE( "Test thread pool with default construction", "[thread_pool]" ){
	thread_pool tp;
	queue<int> numbers(4100);
	std::set<int> trans {};
	CHECK( trans.empty() );
	int tmp;
	std::mutex m;
	CHECK( numbers.is_empty() );
	for( int i = 0; i < 4096; ++i ){
		tp.schedule( [&numbers, i](){int j = i; numbers.push(std::move(j));} );
	}
	for( int i = 0; i < 4096; ++i ){
		tp.schedule( [&numbers, &trans, i, &tmp, &m](){
				numbers.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);
				} );
	}
	tp.shutdown();
	for( int i = 0; i < 4096; ++i )
		CHECK( trans.contains(i) );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1;
	queue<int> number1(2200);
	trans.clear();
	CHECK( trans.empty() );
	CHECK( number1.is_empty() );
	for( int i = 0; i < 2187; ++i ){
		t1.schedule( [&number1, i](){int j = i; number1.push(std::move(j));} );
	}
	for( int i = 0; i < 2187; ++i ){
		t1.schedule( [&number1, &trans, i, &tmp, &m](){number1.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);
				} );
	}
	t1.shutdown();
	for( int i = 0; i < 2187; ++i )
		CHECK( trans.contains(i) );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}

TEST_CASE( "Test thread pool with parametrized construction", "[thread_pool]" ){
	thread_pool tp(128);
	queue<int> numbers(4100);
	std::set<int> trans {};
	CHECK( trans.empty() );
	int tmp;
	std::mutex m;
	CHECK( numbers.is_empty() );
	for( int i = 0; i < 4096; ++i ){
		tp.schedule( [&numbers, i](){int j = i; numbers.push(std::move(j));} );
	}
	for( int i = 0; i < 4096; ++i ){
		tp.schedule( [&numbers, &trans, i, &tmp, &m](){
				numbers.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);} );
	}
	tp.shutdown();
	for( int i = 0; i < 4096; ++i )
		CHECK( trans.contains(i) );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1(7);
	queue<int> number1(2200);
	trans.clear();
	CHECK( trans.empty() );
	CHECK( number1.is_empty() );
	for( int i = 0; i < 2187; ++i ){
		t1.schedule( [&number1, i](){int j = i; number1.push(std::move(j));} );
	}
	for( int i = 0; i < 2187; ++i ){
		t1.schedule( [&number1, &trans, i, &tmp, &m](){
				number1.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);} );
	}
	t1.shutdown();
	for( int i = 0; i < 2187; ++i )
		CHECK( trans.contains(i) );
	CHECK( t1.is_shutdown() );
	CHECK( number1.is_empty() );
}

TEST_CASE( "Test thread pool further with parametrized construction", "[thread_pool]" ){
	thread_pool tp(50);
	queue<int> numbers(4100);
	std::set<int> trans {};
	CHECK( trans.empty() );
	int tmp;
	std::mutex m;
	CHECK( numbers.is_empty() );
	for( int i = 0; i < 4096; ++i ){
		tp.schedule( [&numbers, i](){int j = i; numbers.push(std::move(j));} );
	}
	for( int i = 0; i < 4096; ++i ){
		tp.schedule( [&numbers, &trans, i, &tmp, &m](){
				numbers.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);
				} );
	}
	tp.shutdown();
	for( int i = 0; i < 4096; ++i )
		CHECK( trans.contains(i) );
	CHECK( tp.is_shutdown() );
	CHECK( numbers.is_empty() );
	thread_pool t1(5);
	queue<int> number1(2200);
	trans.clear();
	CHECK( trans.empty() );
	CHECK( number1.is_empty() );
	for( int i = 0; i < 2187; ++i ){
		t1.schedule( [&number1, i](){int j = i; number1.push(std::move(j));} );
	}
	for( int i = 0; i < 2187; ++i ){
		t1.schedule( [&number1, &trans, i, &tmp, &m](){
				number1.pop(tmp);
				std::unique_lock lock(m);
				trans.insert(tmp);} );
	}
	t1.shutdown();
	for( int i = 0; i < 2187; ++i )
		CHECK( trans.contains(i) );
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

